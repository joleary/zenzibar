/*
 * singlton mtp adapter class
 * provides a ingle entry point to the various operations
 * that the mtp device supports
 */
#include <libmtp.h>
#include "mtpDeviceDetails.h"
#include <iostream>
#include <string>
#include <vector>


struct trackItem {	uint32_t parent_id;
					uint32_t id;
					uint32_t storage_id;
					std::string title;
					std::string artist;
					std::string composer;
					std::string album;
					std::string genre;
					std::string date;
					std::string filename;
					uint16_t tracknum;
					uint32_t duration;
					uint16_t rating;
				};

class mtpAdapter {
	private:
		static mtpAdapter* pInstance;
		LIBMTP_mtpdevice_t *device;
		bool connected;
		mtpDeviceDetails *deviceData;
		LIBMTP_folder_t *folders;
		LIBMTP_playlist_t * playlists;
		std::vector<struct trackItem> trackVector;
				
	public:
		static mtpAdapter* Instance();
		bool connect();
		void disconnect();
		mtpDeviceDetails* getDeviceData();
		void printFolderList();
		void printFolderTree();
		void recurseFolder(LIBMTP_folder_t *,int);
		void createTracklist();
		void listFolderTracks(uint32_t);
		void listPlaylists();
		void savePlaylist(uint32_t);
		bool isConnected() {return connected;};
		std::string getDeviceDetailsString() {return (deviceData!=NULL) ? deviceData->toString():"No Device"; }; 
	protected:
		mtpAdapter();
		mtpAdapter(const mtpAdapter&);
		mtpAdapter& operator= (const mtpAdapter&);
};
