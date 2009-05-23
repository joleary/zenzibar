#include "mtpWrapper.h"
#include "gtkzenui.h"
#include "handyStructures.h"
#include <string.h>

int INITIALISED = 0;
int CURRENTDEVICENUMBER=-1;

static void quit(GtkWidget *widget,gpointer data) {
	clearCache();
	disconnect();
	gtk_main_quit();
}

static void populateDeviceList() {
	int i=0;
	for(i=0;i<getNumberOfDevices();i++) {
		addDeviceToDeviceList(i,getDeviceName(i));
	}
}

int trackCachedProgress(uint64_t const sent,uint64_t const total,void const *const data) {
	while(gtk_events_pending()) {
		gtk_main_iteration();
	}
	gtk_progress_bar_pulse(GTK_PROGRESS_BAR(mainProgressBar));
}

static void connectToDevice(GtkWidget *widget,gpointer data) {
	GtkTreeIter iter;
	GtkTreeModel *model;
	gint deviceNumber;
	char *deviceString;
	if(gtk_tree_selection_get_selected(deviceSelect,&model,&iter)) {
		gtk_tree_model_get(model,&iter,0,&deviceNumber,1,&deviceString,-1);
		if(deviceNumber!=CURRENTDEVICENUMBER) {
			CURRENTDEVICENUMBER = connectToZen(deviceNumber);
			gtk_widget_show(mainProgressBar);
			gtk_progress_bar_set_pulse_step(GTK_PROGRESS_BAR(mainProgressBar),0.1);
			if(CURRENTDEVICENUMBER!=-1) {
				cacheTracks((LIBMTP_progressfunc_t)trackCachedProgress);
				statusBarLog("Caching completed");
				char *newDeviceString = (char *)calloc(strlen(getDeviceName(deviceNumber)) + 1,sizeof(char));
				char cacheSizeString[8];
				sprintf(cacheSizeString," (%d)\0",getCacheSize());
				strcat(newDeviceString,deviceString);
				newDeviceString = (char *)realloc(newDeviceString,sizeof(newDeviceString)+8);
				strcat(newDeviceString,cacheSizeString);
				updateDeviceListEntry(iter,newDeviceString);
				free(newDeviceString);
				INITIALISED=1;
			}
			gtk_widget_hide(mainProgressBar);
		}
		if(INITIALISED==1) {
			clearTrackList();
			trackMap *temp = getCache();
			while(temp!=NULL) {
				updateTrackList(temp->details);
				temp = temp->next;
			}
		}
		statusBarLog("Connected");
	}
}

static void disconnectFromDevice(GtkWidget *widget, gpointer data) {
	clearTrackList();
	clearCache();
	if(disconnect()==0) {
		CURRENTDEVICENUMBER  = -1;
		INITIALISED = 0;
	}
	statusBarLog("Disconnected");
}

static gint deviceListHandler(GtkWidget *widget,GdkEvent *event) {
	GdkEventButton *evtButton;
	if(event->type==GDK_BUTTON_PRESS) {
		evtButton = (GdkEventButton *)event;
		if(evtButton->button == 3) {
			gtk_menu_popup(GTK_MENU(widget),NULL,NULL,NULL,NULL,evtButton->button,evtButton->time);
			g_signal_connect(G_OBJECT(devicePopupConnect),"activate",G_CALLBACK(connectToDevice),NULL);
			g_signal_connect(G_OBJECT(devicePopupDisconnect),"activate",G_CALLBACK(disconnectFromDevice),NULL);
			gtk_widget_show_all(widget);
			return TRUE;
		}
	}
	return FALSE;
}

int main(int argc, char*argv[]) {
	int numberofzens;
	int numberoftracks;
	gtk_init(&argc,&argv);
	drawUI();
	numberofzens = initZenLibrary();
	g_signal_connect(G_OBJECT(rootWindow),"destroy",G_CALLBACK(quit),NULL);
	g_signal_connect_swapped(G_OBJECT(sidebarDeviceList),"button_press_event",G_CALLBACK(deviceListHandler),devicePopupMenu);
	populateDeviceList();
	gtk_main();
	cleanUpZenLibrary();
	return 0;
}
