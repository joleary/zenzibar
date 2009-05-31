#include "mtpWrapper.h"
#include "gtkzenui.h"
#include "handyStructures.h"
#include <string.h>
#include "musicLibrary.h"

int INITIALISED = 0;
int CURRENTDEVICENUMBER=-1;

static void threadStatus(GtkWidget *widget,gpointer data) {
	fprintf(stdout,"status is: %s",gtk_label_get_text(GTK_WIDGET(widget)));
}

static void quit(GtkWidget *widget,gpointer data) {
	clearCache();
	clearFolderCache();
	disconnect();
	gtk_main_quit();
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

int main(int argc, char*argv[]) {
	int numberofzens=0;
	gtk_init(&argc,&argv);
	drawUI();
	numberofzens = initZenLibrary();
	g_signal_connect(G_OBJECT(rootWindow),"destroy",G_CALLBACK(quit),NULL);
	//setupFirstDevice();
	beginThread();
	g_signal_connect(G_OBJECT(messageHandler),"notify::label",G_CALLBACK(threadStatus),NULL);
	gtk_main();
	cleanUpZenLibrary();
	if(pthread_cancel(threadId)==0) {
		fprintf(stdout,"thread terminated");
	}
	return 0;
}
