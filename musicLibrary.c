#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <gio/gio.h>
#include <gtk/gtk.h>
#include <id3tag.h>
#include "musicLibrary.h"

#define MUSIC_DIR "/home/joleary/Music\0"

/*
 * private protypes only used within the thread
 */
int recurseDirs(const char *,ThreadManager *);
int addToIndex(const char *,const char *,const char *);
char * getTagString(struct id3_tag *tag,const char *frameName);
libraryEntry *rootMusicLibraryEntry = NULL;
libraryEntry *lastMusicLibraryEntry = NULL;
long musicLibraryTrackCount=0;
pthread_t threadId;
ThreadManager *tm;
int indexReady;
/*
 * consider emitting a signal to show the thread is doing somthing
 * The thread is mostly responsible for building an index
 * so emit at the start of every call 
 */

char * getTagString(struct id3_tag *tag, const char *frameName) {
	struct id3_frame *frame;
	union id3_field *field;
	int i;
	const id3_ucs4_t *unicode;
	char *ret;
	
	frame = id3_tag_findframe(tag,frameName,0);
	if(!frame) {
		fprintf(stdout,"No frame\n");
		return NULL;
	}
	for(i=0;;i++) {
		field = id3_frame_field(frame,i);
		if(!field) {
			break;
		}
		if(id3_field_type(field) == ID3_FIELD_TYPE_STRINGLIST) {
			unicode = id3_field_getstrings(field,0);
			if(unicode) {
				id3_utf8_t *str;
				str = id3_ucs4_utf8duplicate(unicode);
				ret = (char *) str;
			}
			
		}
	}
	
	return ret;
}

trackDetails * getMP3Details(char *path) {
	trackDetails *details;
	details = calloc(1,sizeof(trackDetails));
	struct id3_file *mp3file = NULL;
	struct id3_tag *tag = NULL;
	mp3file = id3_file_open(path,ID3_FILE_MODE_READONLY);
	if(mp3file!=NULL) {
		tag = id3_file_tag(mp3file);
		details->title = getTagString(tag,"TIT2");
		details->artist = getTagString(tag,"TPE1");
		details->album = getTagString(tag,"TALB");
	} else {
			fprintf(stdout,"could not get details for %s\n",path);
	}
	id3_file_close(mp3file);
	
	return details;
}

int recurseDirs(const char *path, ThreadManager *threadman) {
	
	GValue message = {0,};
	g_value_init(&message,G_TYPE_STRING);
	g_value_set_string(&message,"busy");
	g_object_set_property(G_OBJECT(threadman),"status", &message);
	g_value_unset(&message);

	if(path==NULL) {
		return 1;
	}
	sleep(1);
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
					int res = recurseDirs(fullPath,threadman);
					if(res!=0) {
						fprintf(stderr,"Error with %s\n",fullPath);
					}
				} else {			
					const gchar *type = g_file_info_get_attribute_string(finfo,G_FILE_ATTRIBUTE_STANDARD_CONTENT_TYPE);
					if(strcmp(type,"audio/mpeg")==0) {
						addToIndex(name,fullPath,type);
					}
				} 
				free(fullPath);
			}
			finfo = g_file_enumerator_next_file(fileEnumerator,NULL,NULL);	
		}
		g_object_unref(fileEnumerator);
	}
	return 0;
}

int addToIndex(const char *shortName, const char *fullPath,const char *mimeType) {
	if(strcmp(mimeType,"audio/mpeg")!=0) {
		return 1;
	}
	libraryEntry *temp;
	temp = calloc(1,sizeof(libraryEntry));
	temp->fullPath = strdup(fullPath);
	temp->shortName = strdup(shortName);
	temp->mimeType = strdup(mimeType);
	temp->details = getMP3Details(temp->fullPath);
	
	if(rootMusicLibraryEntry==NULL) {
		rootMusicLibraryEntry = temp;
	} else {
		lastMusicLibraryEntry->nextEntry = temp;
	}
	lastMusicLibraryEntry = temp;
	musicLibraryTrackCount++;
	return 0;	
}

void *searchFiles(void *arg) {
	ThreadManager *threadman = (ThreadManager *)arg;
	int res = recurseDirs(MUSIC_DIR,threadman);
	if(res==0) {
		fprintf(stdout,"completed cache\n");
		// make the index available 
		// emit a completed signal
	} else {
		fprintf(stdout,"caching error\n");

		GValue message = {0,};
		g_value_init(&message,G_TYPE_STRING);
		g_value_set_string(&message,"idle");
		g_object_set_property(G_OBJECT(threadman),"status", &message);
		g_value_unset(&message);

		sleep(10);
		free(rootMusicLibraryEntry);
		rootMusicLibraryEntry=NULL;
		// clear the index and
		// then sleep for a while
		// we can try again later
		// emit idle signal until we resume
	}
	fprintf(stdout,"thread exit\n");
	//free(rootMusicLibraryEntry);

	GValue message = {0,};
	g_value_init(&message,G_TYPE_STRING);
	g_value_set_string(&message,"finished");
	g_object_set_property(G_OBJECT(tm),"status", &message);
	g_value_unset(&message);

	indexReady=1;

	pthread_exit(0);
}

void beginThread() {
	int ret;
	tm = g_object_new(THREAD_TYPE_MANAGER,NULL);
	ret = pthread_create(&threadId,NULL,searchFiles,(void *)tm);
}

int isMusicLibraryAvailable() {
	return indexReady;
}

void initMusicLibrary() {
	indexReady = 0;
	// check for a pre-built index
	if(g_file_test("/home/joleary/Projects/zenzibar/mLib.xml",G_FILE_TEST_EXISTS)) {
		// load and parse the index
		// set the index to ready
	} else {
		beginThread();
	}
}

libraryEntry * getFirstTrack() {
	return rootMusicLibraryEntry;
}
