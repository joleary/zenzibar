#include "mtpWrapper.h"
#include "gtkzenui.h"

int INITIALISED = 0;

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
		connectToZen(deviceNumber);
		gtk_widget_show(mainProgressBar);
		gtk_progress_bar_set_pulse_step(GTK_PROGRESS_BAR(mainProgressBar),0.1);
		
		cacheTracks((LIBMTP_progressfunc_t)trackCachedProgress);
		
		gtk_widget_hide(mainProgressBar);
		char *newDeviceString = (char *)calloc(100,sizeof(char));
		char cacheSizeString[8];
		sprintf(cacheSizeString," (%d)\0",getCacheSize());
		strcat(newDeviceString,deviceString);
		strcat(newDeviceString,cacheSizeString);
		updateDeviceList(iter,newDeviceString);
	}
}

int main(int argc, char*argv[]) {
	int numberofzens;
	int numberoftracks;
	gtk_init(&argc,&argv);
	drawUI();
	numberofzens = initZenLibrary();
	g_signal_connect(G_OBJECT(rootWindow),"destroy",G_CALLBACK(quit),NULL);
	g_signal_connect(G_OBJECT(deviceSelect),"changed",G_CALLBACK(connectToDevice),NULL);
	populateDeviceList();
	gtk_main();
	
	return 0;
}
