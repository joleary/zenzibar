#include "handyStructures.h"
#include "threadManager.h"
#include <pthread.h>

extern ThreadManager *tm;

extern pthread_t threadId;

void beginThread();

typedef struct LIBRARYENTRY libraryEntry;

struct LIBRARYENTRY {
	char *fullPath;
	char *shortName;
	char *mimeType;
	libraryEntry *nextEntry;
};

