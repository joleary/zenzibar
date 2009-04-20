#include <gtk/gtk.h>
#include "mtpAdapter.h"
#include "debugConsole.h"
#include <iostream>

GtkWidget *window;
GtkWidget *menubar;
GtkWidget *statusbar;
GtkWidget *rootVBox;
GtkWidget *roothpane;
GtkWidget *rootNotebook;
GtkWidget *connectButton;
GtkWidget *disconnectButton;
GtkWidget *vpane;
GtkWidget *hbox;
GtkWidget *consoleFrame;

mtpAdapter *adapter;
debugConsole *console;

static void connect(GtkWidget *widget, gpointer data) {
	console->print("connecting...");
	gboolean conStatus=false;
	conStatus = adapter->connect();
	if(conStatus) {
		console->print("Connected");
		std::cout << adapter->getDeviceDetailsString()<< std::endl;
	} else {
		console->print("Failed to connect");
	}
}

static void disconnect(GtkWidget *widget, gpointer data) {
	console->print("disconnecting...");
	adapter->disconnect();
	gboolean conStatus=false;
	conStatus = adapter->isConnected();
	if(conStatus) {
		console->print("failed to disconnect");
	} else {
		console->print("disconnected");
	}
}

static void destroy(GtkWidget *widget, gpointer data) {
	if(adapter->isConnected()) {
		disconnect(NULL,NULL);
	}
	gtk_main_quit();
}

/*
 * create all the widgets needed in the ui
 */
void initialiseUI() {
	menubar = gtk_menu_bar_new();
	statusbar = gtk_statusbar_new();
	rootVBox = gtk_vbox_new(false,0);
	roothpane = gtk_hpaned_new();
	vpane = gtk_vpaned_new();
	consoleFrame = gtk_frame_new(NULL);
	console = new debugConsole(GTK_CONTAINER(consoleFrame));
	connectButton = gtk_button_new_with_label("Connect");
	disconnectButton = gtk_button_new_with_label("DisConnect");
	hbox = gtk_hbox_new(false,10);
}

/*
 * arrange the ui widgets in the window
 */
void assembleUI(GtkWidget *topLevelWindow) {
	
	gtk_container_add(GTK_CONTAINER(topLevelWindow),rootVBox);
	
	gtk_box_pack_start(GTK_BOX(rootVBox),menubar,false,true,0);
	
	gtk_box_pack_start(GTK_BOX(rootVBox),roothpane,true,true,0);
	
	gtk_box_pack_start(GTK_BOX(rootVBox),statusbar,false,true,0);
	
	gtk_paned_add2(GTK_PANED(roothpane),vpane);
	
	gtk_paned_add2(GTK_PANED(vpane),consoleFrame);
	
	gtk_box_pack_start(GTK_BOX(hbox),connectButton,false,false,5);
	
	gtk_box_pack_start(GTK_BOX(hbox),disconnectButton,false,false,5);
	
	gtk_paned_add1(GTK_PANED(vpane),hbox);
}

void connectSignals() {
	g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(destroy), NULL);
	g_signal_connect(G_OBJECT(connectButton),"clicked",G_CALLBACK(connect),NULL);
	g_signal_connect(G_OBJECT(disconnectButton),"clicked",G_CALLBACK(disconnect),NULL);
}

int main(int argc, char *argv[]) {
	
	gtk_init(&argc,&argv);
	
	adapter = mtpAdapter::Instance();
	
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	
	initialiseUI();
	
	assembleUI(window);
	
	connectSignals();
	
	gtk_widget_show_all(window);
	
	gtk_main();
	
	return 0;
}
