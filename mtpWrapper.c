#include "mtpWrapper.h"

LIBMTP_raw_device_t * rawDevices;
LIBMTP_mtpdevice_t *device;
int numRawDevices = 0;
deviceDetails * details;
trackMap * rootTrackList = NULL;
trackMap * lastTrack = NULL;
uint32_t cacheCount = 0;
folderTree * rootFolderTree = NULL;

/*
 * add a track to the cache at the end of the list
 */
int cacheTrack (LIBMTP_track_t *track) {
	if(track) {
		if(track->item_id) {
			trackMap *temp;
			trackDetails *tempTrack;
			temp = malloc(sizeof(trackMap));
			temp->uid = cacheCount;
			tempTrack = malloc(sizeof(trackDetails));
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
			fprintf(stdout,"Cache number %d\n",temp->uid);
			if(!rootTrackList) {
				rootTrackList = temp;
			} else {
				lastTrack->next = temp;
			}
			lastTrack = temp;
			cacheCount++;
			return 0;
		}
		return 1;
	}
	return 1;
}

/*
 * free memory used by the cache starting from the node passed
 * pass in the root node to free the entire cache
 */
int clearCache() {
	if(cacheCount!=0) {
		cacheCount=0;
		trackMap *temp=NULL;
		while(rootTrackList!=NULL) {
			if(rootTrackList->details) {
				free(rootTrackList->details);
			}
			temp = rootTrackList->next;
			free(rootTrackList);
			rootTrackList=temp;
		}
		rootTrackList=NULL;
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
			break;
		case LIBMTP_ERROR_CONNECTING:
			fprintf(stderr,"Could not connect to a device\n");
			break;
		case LIBMTP_ERROR_MEMORY_ALLOCATION:
			fprintf(stderr,"Could not allocate memory for the device(s) attached\n");
			break;
		case LIBMTP_ERROR_NONE:
			return numRawDevices;
		case LIBMTP_ERROR_GENERAL:
		default:
			fprintf(stderr,"Unknown libtmp error\n");
			break;
	}
	return 0;
}

int connectToZen(int deviceNumber) {
	if(deviceNumber<numRawDevices) {
		if(device!=NULL) {
			LIBMTP_Release_Device(device);
		}
		device = LIBMTP_Open_Raw_Device(&rawDevices[deviceNumber]);
		return deviceNumber;
	} else {
		return -1;
	}
}

int disconnect() {
	//if(device) {
	//	LIBMTP_Release_Device(device);
	//}
	return 0;
}

int cacheTracks(LIBMTP_progressfunc_t progressFunction) {
	int cachedAmount=0;
	if(device!=NULL) {
		LIBMTP_track_t *tracklist;
		tracklist = LIBMTP_Get_Tracklisting_With_Callback(device,progressFunction,NULL);
		while(tracklist!=NULL) {
			LIBMTP_track_t *temp;
			if(cacheTrack(tracklist)==0) {
				cachedAmount++;
			}
			temp = tracklist;
			tracklist = tracklist->next;
			LIBMTP_destroy_track_t(temp);
		}
		LIBMTP_destroy_track_t(tracklist);
	}
	return cachedAmount;
}

folderTree * addFolder(LIBMTP_folder_t *);

folderTree* addFolder(LIBMTP_folder_t *folder) {
	folderTree *Position = malloc(sizeof(folderTree));
	if(folder->folder_id==0) {
		return NULL;
	}
	Position->name = stringStore(folder->name);
	fprintf(stdout,"%s\n",Position->name);
	Position->uid = folder->folder_id;
	Position->parentUid = folder->parent_id;
	if(folder->child!=NULL) {
		Position->child = malloc(sizeof(folderTree));
		Position->child = addFolder(folder->child);
	} else {
		Position->child = NULL;
	}
	if(folder->sibling!=NULL) {
		Position->sibling = malloc(sizeof(folderTree));
		Position->sibling = addFolder(folder->sibling);
	} else {
		Position->sibling=NULL;
	}
	return Position;
}

int cacheFolders() {
	if(device) {
		rootFolderTree = malloc(sizeof(folderTree));
		rootFolderTree->name = stringStore(LIBMTP_Get_Friendlyname(device));
		rootFolderTree->uid=0;
		rootFolderTree->parentUid=0;
		rootFolderTree->sibling=NULL;
		LIBMTP_folder_t *root = LIBMTP_Get_Folder_List(device);
		if(root!=NULL) {
			rootFolderTree->child = addFolder(root);
		} else {
			rootFolderTree->child=NULL;
		}
		LIBMTP_destroy_folder_t(root);
	}
	return 0;
}

int getNumberOfDevices() {
	return numRawDevices;
}

const char * getDeviceName(int deviceNumber) {
	char *deviceName=NULL;
	if(deviceNumber < numRawDevices) {
		deviceName = rawDevices[deviceNumber].device_entry.product;
	}
	return deviceName;
}

int getCacheSize() {
	return cacheCount;
}

trackMap * getCache() {
	return rootTrackList;
}

int cleanUpZenLibrary() {
	if(device) {
		LIBMTP_Release_Device(device);
	}
	if(rawDevices) {
		free(rawDevices);
	}
}

void clearFolderCache() {
	if(rootFolderTree) {
		free(rootFolderTree);
	}
}

folderTree * getRootFolder() {
	return rootFolderTree;
}
