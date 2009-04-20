#include "mtpAdapter.h"
#include <string.h>
#include <iostream>
#include <string>
#include <vector>
#include <libmtp.h>

using namespace std ;

mtpAdapter* mtpAdapter::Instance() {
	if(mtpAdapter::pInstance==0) {
			mtpAdapter::pInstance = new mtpAdapter();
	}
	return mtpAdapter::pInstance;
}

/*
 * Constructer starts up the mtp library
 * and creates the initial connection
 * 
 */
mtpAdapter::mtpAdapter() {
	cout << "create"<< endl;
	deviceData = new mtpDeviceDetails();
	connected=false;
	LIBMTP_Init();
	LIBMTP_raw_device_t * rawDevices;
	int numRawDevice;
	LIBMTP_Detect_Raw_Devices(&rawDevices,&numRawDevice);
	cout << "found: " << numRawDevice << endl;
}

/*
 * temp method to print the folders
 * at the root of the device
 */

void mtpAdapter::printFolderList() {
	if(connected) {
		if(folders!=NULL) {
			LIBMTP_folder_t *tempFolder;
			tempFolder = folders;
			while(tempFolder!=NULL) {
				cout << tempFolder->name << endl;
				tempFolder = tempFolder->sibling;
			}
		}
	}
}

/*
 * temp funtion to print
 * the folder tree
 */

void mtpAdapter::printFolderTree() {
	if(connected) {
		if(folders!=NULL) {
			LIBMTP_folder_t *tempFolder;
			tempFolder = folders;
			cout << "ROOT" << endl;
			recurseFolder(tempFolder,0);
		}
	}
}

void mtpAdapter::recurseFolder(LIBMTP_folder_t *fold,int depth) {
	LIBMTP_folder_t *root;
	root = fold;
	while(root!=NULL) {
		for(int i=0;i<depth;i++) {
				cout << "   ";
		} 
		cout << "|-" << root->name << "(" << root->folder_id << ")" << endl;
		listFolderTracks(root->folder_id);
		if(root->child!=NULL) {
			recurseFolder(root->child,depth+1);
		}
		root=root->sibling;
	}
}

/*
 * Test for an existing connection
 * otherwise create a connection
 * return true if we can connect
 */
bool mtpAdapter::connect() {
	if(connected) {
		return true;
	}
	device = LIBMTP_Get_First_Device();
	if(device!=NULL) {	
		connected=true;
		folders = LIBMTP_Get_Folder_List(device);
		playlists = LIBMTP_Get_Playlist_List(device);
		createTracklist();
		cout << "got a device connection" << endl;
		deviceData->setManufacturer(LIBMTP_Get_Manufacturername(device));
		deviceData->setModel(LIBMTP_Get_Modelname(device));
		deviceData->setSerial(LIBMTP_Get_Serialnumber(device));
		deviceData->setVersion(LIBMTP_Get_Deviceversion(device));
		deviceData->setName(LIBMTP_Get_Friendlyname(device));
		return true;
	}
	
	return false;
}

/*
 * print out the tracks in a folder
 */
void mtpAdapter::listFolderTracks(uint32_t folderId) {
	vector<trackItem>::iterator trackIterator;
	trackIterator = trackVector.begin();
	while(trackIterator != trackVector.end()) {
		trackItem tmp = *trackIterator;
		if(tmp.folder==folderId) {
			cout <<  tmp.file << "(" << tmp.title << ")"<< endl;
		}
		++trackIterator;
	}
}

/*
 * Test for an existing connection
 * then disconnect it
 */

void mtpAdapter::disconnect() {
	if(connected) {
		LIBMTP_destroy_folder_t(folders);
		trackVector.clear();
		if(device!=NULL) {
			LIBMTP_Release_Device(device);
			cout << "device released" << endl;
			connected=false;
		}
	} else {
		cout << "not connected, no device to release" << endl;
	}
}

/*
 * gather the device data
 * return it as a string that
 * can be easily parsed
 */

mtpDeviceDetails* mtpAdapter::getDeviceData() {
	deviceData->setManufacturer(LIBMTP_Get_Manufacturername(device));
	return deviceData;
}

/*
 * Populate the tracklist vector for easy "folder"
 * or collection association
 */

void mtpAdapter::createTracklist() {
	LIBMTP_track_t *tracklist;
	tracklist = LIBMTP_Get_Tracklisting_With_Callback(device,NULL,NULL);
	int count=0;
	while(tracklist!=NULL) {
		count++;
		LIBMTP_track_t *tmp;
		tmp = tracklist;
		struct trackItem ti;
		ti.folder = tmp->parent_id;
		ti.file = tmp->item_id;
		if(tmp->title!=NULL) {
			ti.title=tmp->title;
		} else {
			ti.title="Unknown Title";
		}
		trackVector.push_back(ti);
		tracklist = tracklist->next;
		LIBMTP_destroy_track_t(tmp);
	}
	cout << "found:" << count << endl;
}

/*
 * list the playlists on the device
 */

void mtpAdapter::listPlaylists() {
	LIBMTP_playlist_t *tmp;
	tmp = playlists;
	while(tmp!=NULL) {
		cout << tmp->name << endl;
		uint32_t numTracks = tmp->no_tracks;
		for(uint32_t i=0;i<numTracks;i++) {
			struct trackItem ti;
			vector<trackItem>::iterator trackIterator;
			trackIterator = trackVector.begin();
			while(trackIterator!=trackVector.end()) {
				ti=*trackIterator;
				if(ti.file == tmp->tracks[i]) {
					break;
				}
				++trackIterator;
			}
			cout << "\t" << ti.title << endl;
		}
		tmp=tmp->next;
	}
}

/*
 * Always start with a null instance
 * or the singleton pattern falls apart
 */
mtpAdapter* mtpAdapter::pInstance=0;
