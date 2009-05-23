#include <libmtp.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "handyStructures.h"

typedef struct DEVICEDETAILS deviceDetails;

struct DEVICEDETAILS {
	char *friendlyName;
	char *manufacturer;
	char *model;
	char *serialNum;
	char *version;
};

typedef struct TRACKMAP trackMap;

struct TRACKMAP {
	uint32_t uid;
	trackDetails *details;
	trackMap *next;
};
/*
 * function prototypes
 */
int cacheTrack (LIBMTP_track_t *);
int clearCache();
trackDetails * getDetails(uint32_t);
int initZenLibrary();
int connectToZen(int);
int disconnect();
int cacheTracks(LIBMTP_progressfunc_t);
int getNumberOfDevices();
const char * getDeviceName(int);
int getCacheSize();
trackMap * getCache();
