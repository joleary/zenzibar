#ifndef _HANDY_STRUCTURES_
#define _HANDY_STRUCTURES_
#include <stdint.h>
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

typedef struct FOLDERTREE folderTree;

struct FOLDERTREE {
	uint32_t uid;
	uint32_t parentUid;
	char *name;
	folderTree *child;
	folderTree *sibling;
};

char * stringStore(const char *);

#endif
