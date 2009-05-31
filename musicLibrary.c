#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <gio/gio.h>
#include <gtk/gtk.h>
#include "musicLibrary.h"

#define MUSIC_DIR "/home/joleary/Download\0"

GtkWidget *messageHandler;
/*
 * private protypes only used within the thread
 */
int recurseDirs(const char *);
int addToIndex(const char *,const char *,const char *,GFileInfo *);
libraryEntry *rootMusicLibraryEntry = NULL;
libraryEntry *lastMusicLibraryEntry = NULL;
long musicLibraryTrackCount=0;
pthread_t threadId;

/*
 * consider emitting a signal to show the thread is doing somthing
 * The thread is mostly responsible for building an index
 * so emit at the start of every call 
 */

int recurseDirs(const char *path) {
	gtk_label_set_text(GTK_LABEL(messageHandler),"busy");
	sleep(1);
	if(path==NULL) {
		return 1;
	}
	GFile *dir = g_file_new_for_path(path);
	GFileEnumerator *fileEnumerator;
	GError *errorHandler=NULL;
	fileEnumerator = g_file_enumerate_children(dir,"standard::*",G_FILE_QUERY_INFO_NONE,NULL,&errorHandler);
	if(errorHandler!=NULL) {
		fprintf(stderr,"ERROR: %s\n",errorHandler->message);
		g_error_free(errorHandler);
		return 1;
	}
	if(fileEnumerator!=NULL) {
		GFileInfo *finfo = g_file_enumerator_next_file(fileEnumerator,NULL,NULL);
		while(finfo!=NULL) {
			if(!g_file_info_get_is_hidden(finfo)) {
				const gchar *name;
				char *fullPath;
				
				name = g_file_info_get_name(finfo);
				fullPath = strdup(path);
				fullPath = realloc(fullPath,strlen(path)+3+strlen(name));
				strcat(fullPath,"/");
				strcat(fullPath,name);
				
				if(g_file_info_get_file_type(finfo)==G_FILE_TYPE_DIRECTORY) {
					int res = recurseDirs(fullPath);
					if(res!=0) {
						fprintf(stderr,"Error with %s\n",fullPath);
					}
				} else {			
					const char *type = g_file_info_get_attribute_string(finfo,G_FILE_ATTRIBUTE_STANDARD_CONTENT_TYPE);
					addToIndex(name,fullPath,type,finfo);
					free(type);
				} 
				free(fullPath);
				g_free(name);
			}
			finfo = g_file_enumerator_next_file(fileEnumerator,NULL,NULL);	
		}
		g_object_unref(fileEnumerator);
	}
	return 0;
}

int addToIndex(const char *shortName, const char *fullPath,const char *mimeType, GFileInfo *finfo) {
	libraryEntry *temp;
	temp = malloc(sizeof(libraryEntry));
	temp->fullPath = strdup(fullPath);
	temp->shortName = strdup(shortName);
	temp->mimeType = strdup(mimeType);
	if(rootMusicLibraryEntry==NULL) {
		rootMusicLibraryEntry = temp;
	} else {
		lastMusicLibraryEntry->nextEntry = temp;
	}
	lastMusicLibraryEntry = temp;
	musicLibraryTrackCount++;
	if(strcmp(mimeType,"audio/mpeg")==0) {
		fprintf(stdout,"Added %s\n",shortName);
	}
	
}

void *searchFiles(void *arg) {
	int res = recurseDirs(MUSIC_DIR);
	if(res==0) {
		fprintf(stdout,"completed cache\n");
		// make the index available 
		// emit a completed signal
	} else {
		fprintf(stdout,"caching error\n");
		gtk_label_set_text(GTK_LABEL(messageHandler),"idle");
		sleep(10);
		free(rootMusicLibraryEntry);
		rootMusicLibraryEntry=NULL;
		// clear the index and
		// then sleep for a while
		// we can try again later
		// emit idle signal until we resume
	}
	fprintf(stdout,"thread exit\n");
	free(rootMusicLibraryEntry);
	
	gtk_label_set_text(GTK_LABEL(messageHandler),"finshed");
	pthread_exit(0);
}

void beginThread() {
	int ret;
	messageHandler = gtk_widget_new(GTK_TYPE_LABEL,"label","created",NULL);
	ret = pthread_create(&threadId,NULL,searchFiles,NULL);
}
