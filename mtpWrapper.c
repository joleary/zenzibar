#include "mtpWrapper.h"

LIBMTP_raw_device_t * rawDevices;
LIBMTP_mtpdevice_t *device;
int numRawDevices = 0;
deviceDetails * details;
trackMap * rootTrackList = NULL;
trackMap * lastTrack = NULL;
uint32_t cacheCount = 0;

/*
 * handy function to copy one string to another
 */
char * stringStore(const char *src) {
	char *store;
	if(src!=NULL) {
		store = strdup(src); 
	} else {
		store = strdup("unknown\0");
	}
	return store;
}

/*
 * add a track to the cache at the end of the list
 */
void cacheTrack (LIBMTP_track_t *track) {
	if(track!=NULL) {
		if(track->item_id) {
			trackMap *temp;
			trackDetails *tempTrack;
			temp = (trackMap *)malloc(sizeof(trackMap));
			temp->uid = cacheCount;
			tempTrack = (trackDetails *)malloc(sizeof(trackDetails));
			tempTrack->id = track->item_id;
			tempTrack->parentid = track->parent_id;
			tempTrack->title = stringStore(track->title);
			tempTrack->artist = stringStore(track->artist);
			tempTrack->album = stringStore(track->album);
			tempTrack->composer = stringStore(track->composer);
			tempTrack->genre = stringStore(track->genre);
			tempTrack->date = stringStore(track->date);
			tempTrack->filename = stringStore(track->filename);
			tempTrack->duration = track->duration;
			tempTrack->rating = track->rating;
			temp->details = tempTrack;
			temp->next = NULL;
			if(!rootTrackList) {
				rootTrackList = temp;
			} else {
				lastTrack->next = temp;
			}
			lastTrack = temp;
			cacheCount++;
		}
	}
}

/*
 * free memory used by the cache starting from the node passed
 * pass in the root node to free the entire cache
 */
int clearCache() {
	int count=0;
	trackMap *temp=NULL;
	while(rootTrackList!=NULL) {
		if(rootTrackList->details) {
			free(rootTrackList->details);
		}
		temp = rootTrackList->next;
		free(rootTrackList);
		rootTrackList=temp;
	}
	return 0;
}

/*
 * retrieve track details from the cache by id
 */
trackDetails * getDetails(uint32_t trackid) {
	trackMap *temp;
	temp = rootTrackList;
	while(temp->details->id!=trackid) {
		temp = temp->next;
	}
	return temp->details;
}

int initZenLibrary() {
	LIBMTP_Init();
	LIBMTP_error_number_t err;
	err = LIBMTP_Detect_Raw_Devices(&rawDevices,&numRawDevices);
	switch(err) {
		case LIBMTP_ERROR_NO_DEVICE_ATTACHED:
			fprintf(stdout,"No devices attached\n");
			return 0;
		case LIBMTP_ERROR_CONNECTING:
			fprintf(stderr,"Could not connect to a device\n");
			return 0;
		case LIBMTP_ERROR_MEMORY_ALLOCATION:
			fprintf(stderr,"Could not allocate memory for the device(s) attached\n");
			return 0;
		case LIBMTP_ERROR_NONE:
			return numRawDevices;
		case LIBMTP_ERROR_GENERAL:
		default:
			fprintf(stderr,"Unknown libtmp error\n");
			return 0;
	}
}

int connectToZen(int deviceNumber) {
	if(deviceNumber<numRawDevices) {
		device = LIBMTP_Open_Raw_Device(&rawDevices[deviceNumber]);
		return deviceNumber;
	} else {
		return 0;
	}
}

int disconnect() {
	if(device!=NULL) {
		LIBMTP_Release_Device(device);
	}
}

int cacheTracks(LIBMTP_progressfunc_t progressFunction) {	
	if(device!=NULL) {
		LIBMTP_track_t *tracklist;
		tracklist = LIBMTP_Get_Tracklisting_With_Callback(device,progressFunction,NULL);
		while(tracklist!=NULL) {
			LIBMTP_track_t *temp;
			cacheTrack(tracklist);
			temp = tracklist;
			tracklist = tracklist->next;
			LIBMTP_destroy_track_t(temp);
		}
	}
	return cacheCount;
}

int getNumberOfDevices() {
	return numRawDevices;
}

char * getDeviceName(int deviceNumber) {
	char *deviceName=NULL;
	if(deviceNumber < numRawDevices) {
		deviceName = rawDevices[deviceNumber].device_entry.product;
	}
	return deviceName;
}
