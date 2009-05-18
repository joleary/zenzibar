#include "mtpWrapper.h"
#include "gtkzenui.h"

int INITIALISED = 0;

static void quit(GtkWidget *widget,gpointer data) {
	clearCache();
	disconnect();
	gtk_main_quit();
}

static void populateDeviceList() {
	fprintf(stdout,"populate called");
	int i=0;
	for(i=0;i<getNumberOfDevices();i++) {
		addDeviceToDeviceList(i,getDeviceName(i));
	}
}

int trackCachedProgress(uint64_t const sent,uint64_t const total,void const *const data) {
	fprintf(stdout,"Progress: %d/%d \n",sent,total);
}

static void connectToDevice(GtkWidget *widget,gpointer data) {
	GtkTreeIter iter;
	GtkTreeModel *model;
	gint deviceNumber;
	if(gtk_tree_selection_get_selected(deviceSelect,&model,&iter)) {
		gtk_tree_model_get(model,&iter,0,&deviceNumber,-1);
		fprintf(stdout,"You selected device %d\n",deviceNumber);
		connectToZen(deviceNumber);
		cacheTracks((LIBMTP_progressfunc_t)trackCachedProgress);
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
