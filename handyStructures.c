#include "handyStructures.h"
#include <string.h>
/*
 * handy function to copy one string to another
 */
char * stringStore(const char *src) {
	char *store;
	if(src) {
		store = strdup(src); 
	} else {
		store = strdup("unknown\0");
	}
	return store;
}
