#include "mtpWrapper.h"
#include "gtkzenui.h"
#include "handyStructures.h"
#include <string.h>
#include <glib-2.0/glib.h>
#include "musicLibrary.h"

int CURRENT_STATE = 0;

int INITIALISED = 0;
int CURRENTDEVICENUMBER=-1;
int RUNNING = 0;

int musicLibraryLoaded=0;

enum {EMPTY, INIT, MAIN_LOOP, CLEANUP};

static void threadStatus(GObject *object,gpointer data) {
	GValue message = {0,};
	g_value_init(&message,G_TYPE_STRING);
	g_object_get_property(G_OBJECT(tm),"status", &message);
	const gchar *statusMessage = g_value_get_string(&message);
	
	if(strcmp(statusMessage,"busy")==0) {
		gdk_threads_enter();
			gtk_progress_bar_pulse(GTK_PROGRESS_BAR(mainProgressBar));
		gdk_threads_leave();
	} else {
		gdk_threads_enter();
			gtk_widget_hide(mainProgressBar);
		gdk_threads_leave();
	}
	g_value_unset(&message);
}

void populateLibraryView() {
	libraryEntry *temp = getFirstTrack();
	while(temp!=NULL) {
		fprintf(stdout,"%s\n",temp->details->title);
		temp = temp->nextEntry;
	}
	// loop through the library and populate the view
}

static void quit(GtkWidget *widget,gpointer data) {
	clearCache();
	clearFolderCache();
	disconnect();
	CURRENT_STATE = CLEANUP;
}

void setupFirstDevice() {
	connectToDevice();
	populateDeviceTree(getRootFolder());
	
	if(INITIALISED==1) {
		trackMap *temp = getCache();
		trackMap *hold;
		while(temp!=NULL) {
			if(temp->details!=NULL) {
				updateTrackList(temp->details);
			}
			hold = temp;
			temp = hold->next;
		}
		gtk_tree_view_set_search_column(GTK_TREE_VIEW(trackList),1);
	}
}

int trackCachedProgress(uint64_t const sent,uint64_t const total,void const *const data) {
	while(gtk_events_pending()) {
		gtk_main_iteration();
	}
	char sentAmount[20];
	sprintf(sentAmount,"Found %d",sent);
	gtk_progress_bar_set_text(GTK_PROGRESS_BAR(mainProgressBar),(const char *)sentAmount);
	gtk_progress_bar_pulse(GTK_PROGRESS_BAR(mainProgressBar));
	while(gtk_events_pending()) {
		gtk_main_iteration_do(FALSE);
	};
}

int connectToDevice() {
	
	CURRENTDEVICENUMBER = connectToZen(0);
	gtk_widget_show(mainProgressBar);
	gtk_progress_bar_set_pulse_step(GTK_PROGRESS_BAR(mainProgressBar),0.01);
	if(CURRENTDEVICENUMBER!=-1) {
		cacheTracks((LIBMTP_progressfunc_t)trackCachedProgress);
		statusBarLog("Caching tracks completed");
		INITIALISED=1;
	} else {
		return FALSE;
	}
	gtk_widget_hide(mainProgressBar);
	cacheFolders();
	statusBarLog("Caching folders completed");
	return TRUE;
}

static void disconnectFromDevice(GtkWidget *widget, gpointer data) {
	clearTrackList();
	clearCache();
	clearFolderCache();
	if(disconnect()==0) {
		CURRENTDEVICENUMBER  = -1;
		INITIALISED = 0;
	}
	statusBarLog("Disconnected");
}

int initZenzibar() {
	initZenLibrary();
	buildUI();
	initMusicLibrary();
	
	g_signal_connect(G_OBJECT(rootWindow),"destroy",G_CALLBACK(quit),NULL);
	
	showUI();
	
	if(getNumberOfDevices() > 0 ) {
		setupFirstDevice();
	}
	
	musicLibraryLoaded=0;
	
	CURRENT_STATE = MAIN_LOOP;
}

int mainLoop() {
	if((musicLibraryLoaded==0) && (isMusicLibraryAvailable()==1)) {
		populateLibraryView();
		musicLibraryLoaded=1;
	}
	while(gtk_events_pending()) {
		gtk_main_iteration_do(FALSE);
	};
}

int cleanUp() {
	cleanUpZenLibrary();
	if(pthread_cancel(threadId)==0) {
		fprintf(stdout,"thread terminated");
	}
	RUNNING = 1;
}

int main(int argc, char*argv[]) {
	
	gtk_init(&argc,&argv);
	
	CURRENT_STATE = INIT;
	
	while(RUNNING==0) {
		switch (CURRENT_STATE) {
			case INIT: initZenzibar(); 
				break;
			case MAIN_LOOP: mainLoop(); 
				break;
			case CLEANUP: cleanUp();
				break;
		}
	}
	return 0;
}
