#include "handyStructures.h"
#include <pthread.h>



extern pthread_t threadId;

void beginThread();

typedef struct LIBRARYENTRY libraryEntry;

struct LIBRARYENTRY {
	char *fullPath;
	char *shortName;
	char *mimeType;
	libraryEntry *nextEntry;
};

extern GtkWidget *messageHandler;
