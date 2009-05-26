#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <gio/gio.h>
#include "musicLibrary.h"

#define MUSIC_DIR "/home/joleary/Music"

pthread_t threadId;

int recurseDirs(GFile *trunk) {
	GFileEnumerator *fileEnumerator;
	GError *errorHandler;
	fileEnumerator = g_file_enumerate_children(trunk,"standard::*",G_FILE_QUERY_INFO_NONE,NULL,&errorHandler);
	if(fileEnumerator) {
		GFileInfo *finfo = g_file_enumerator_next_file(fileEnumerator,NULL,NULL);
		while(finfo!=NULL) {
			if(!g_file_info_get_is_hidden(finfo)) {
				if(!(g_file_info_get_file_type(finfo)==G_FILE_TYPE_DIRECTORY)) {
					fprintf(stdout,"%s\n",g_file_info_get_attribute_string(finfo,G_FILE_ATTRIBUTE_STANDARD_DISPLAY_NAME));
				} else {
					fprintf(stdout,"DIRECTORY\n");
				}
			}
			finfo = g_file_enumerator_next_file(fileEnumerator,NULL,NULL);	
		}
	}
	return 0;
}

void *searchFiles(void *arg) {
	GFile *mDir = g_file_new_for_path("/home/joleary");
	recurseDirs(mDir);
	fprintf(stdout,"thread exit\n");
	pthread_exit(0);
}

void beginThread() {
	int ret;
	ret = pthread_create(&threadId,NULL,searchFiles,NULL);
}
