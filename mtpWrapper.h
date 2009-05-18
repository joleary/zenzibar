#include <libmtp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

typedef struct DEVICEDETAILS deviceDetails;

struct DEVICEDETAILS {
	char *friendlyName;
	char *manufacturer;
	char *model;
	char *serialNum;
	char *version;
};

typedef struct TRACKDETAILS trackDetails;

struct TRACKDETAILS {
	uint32_t id;
	uint32_t parentid;
	char *title;
	char *artist;
	char *album;
	char *composer;
	char *genre;
	char *date;
	char *filename;
	uint16_t rating;
	uint32_t duration;
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
void cacheTrack (LIBMTP_track_t *);
int clearCache();
trackDetails * getDetails(uint32_t);
int initZenLibrary();
int connectToZen(int);
int disconnect();
int cacheTracks(LIBMTP_progressfunc_t);
int getNumberOfDevices();
char * getDeviceName(int);
int getCacheSize();
