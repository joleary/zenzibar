#include "gtkzenui.h"

GtkWidget *rootWindow;
GtkWidget *rootVBox;
GtkWidget *windowMenu;
GtkWidget *windowFooter;
GtkWidget *mainContainerHPane;
GtkWidget *mainSideBarScrollContainer;
GtkWidget *mainProgressBar;
GtkWidget *mainScrollContainer;

GtkWidget *sidebarDeviceList;
GtkListStore *deviceListStore;
GtkTreeViewColumn *deviceColumn;
GtkTreeSelection *deviceSelect;

GtkWidget *trackList;
GtkListStore *trackListStore;
GtkTreeViewColumn *titleColumn;
GtkTreeViewColumn *artistColumn;
GtkTreeViewColumn *albumColumn;
GtkTreeViewColumn *genreColumn;
GtkTreeViewColumn *durationColumn;
GtkTreeViewColumn *ratingColumn;
GtkTreeSelection *trackSelect;

enum {ID,TITLE,ARTIST,ALBUM,GENRE,DURATION,RATING,TRACK_COLUMNS};

void drawUI() {
	rootWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	rootVBox = gtk_vbox_new(FALSE,0);
	windowMenu = gtk_menu_bar_new();
	windowFooter = gtk_statusbar_new();
	mainContainerHPane = gtk_hpaned_new();
	mainSideBarScrollContainer = gtk_scrolled_window_new(NULL, NULL);
	
	deviceListStore = gtk_list_store_new(2, G_TYPE_INT, G_TYPE_STRING);
	sidebarDeviceList = gtk_tree_view_new_with_model(GTK_TREE_MODEL(deviceListStore));
	deviceColumn = gtk_tree_view_column_new_with_attributes("Devices", gtk_cell_renderer_text_new(), "text", 1, NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(sidebarDeviceList), deviceColumn);
	deviceSelect = gtk_tree_view_get_selection(GTK_TREE_VIEW(sidebarDeviceList));
	gtk_tree_selection_set_mode(deviceSelect, GTK_SELECTION_SINGLE);
	
	mainScrollContainer = gtk_scrolled_window_new(NULL,NULL);
	
	trackListStore = gtk_list_store_new(TRACK_COLUMNS, G_TYPE_INT,
														G_TYPE_STRING,
														G_TYPE_STRING,
														G_TYPE_STRING,
														G_TYPE_STRING,
														G_TYPE_STRING,
														G_TYPE_INT);
	trackList = gtk_tree_view_new_with_model(GTK_TREE_MODEL(trackListStore));
	titleColumn = gtk_tree_view_column_new_with_attributes("title",gtk_cell_renderer_text_new(),"text",1,NULL);
	artistColumn = gtk_tree_view_column_new_with_attributes("artist",gtk_cell_renderer_text_new(),"text",2,NULL);
	albumColumn = gtk_tree_view_column_new_with_attributes("album",gtk_cell_renderer_text_new(),"text",3,NULL);
	genreColumn = gtk_tree_view_column_new_with_attributes("genre",gtk_cell_renderer_text_new(),"text",4,NULL);
	durationColumn = gtk_tree_view_column_new_with_attributes("duration",gtk_cell_renderer_text_new(),"text",5,NULL);
	ratingColumn = gtk_tree_view_column_new_with_attributes("rating",gtk_cell_renderer_text_new(),"text",6,NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(trackList),titleColumn);
	gtk_tree_view_append_column(GTK_TREE_VIEW(trackList),artistColumn);
	gtk_tree_view_append_column(GTK_TREE_VIEW(trackList),albumColumn);
	gtk_tree_view_append_column(GTK_TREE_VIEW(trackList),genreColumn);
	gtk_tree_view_append_column(GTK_TREE_VIEW(trackList),durationColumn);
	gtk_tree_view_append_column(GTK_TREE_VIEW(trackList),ratingColumn);
	trackSelect = gtk_tree_view_get_selection(GTK_TREE_VIEW(trackList));
	gtk_tree_selection_set_mode(trackSelect, GTK_SELECTION_SINGLE);
	
	mainProgressBar = gtk_progress_bar_new();
	arrangeUI();
	showUI();
}

void arrangeUI() {
	gtk_container_add(GTK_CONTAINER(rootWindow),rootVBox);
	gtk_box_pack_start(GTK_BOX(rootVBox),windowMenu,FALSE,FALSE,0);
	gtk_box_pack_start(GTK_BOX(rootVBox),mainContainerHPane,TRUE,TRUE,0);
	gtk_box_pack_start(GTK_BOX(rootVBox),windowFooter,FALSE,FALSE,0);
	gtk_box_pack_start(GTK_BOX(windowFooter),mainProgressBar,FALSE,FALSE,0);
	gtk_box_reorder_child(GTK_BOX(windowFooter),mainProgressBar,0);
	gtk_paned_add1(GTK_PANED(mainContainerHPane),mainSideBarScrollContainer);
	gtk_paned_add2(GTK_PANED(mainContainerHPane),mainScrollContainer);
	gtk_container_add(GTK_CONTAINER(mainSideBarScrollContainer),sidebarDeviceList);
	gtk_container_add(GTK_CONTAINER(mainScrollContainer),trackList);
}

void showUI() {
	GdkScreen *screen = gtk_window_get_screen(GTK_WINDOW(rootWindow));
	gtk_window_set_default_size(GTK_WINDOW(rootWindow),gdk_screen_get_width(screen)/2,gdk_screen_get_height(screen)/2);
	gtk_widget_show_all(rootWindow);
	gtk_widget_hide(mainProgressBar);
}

void addDeviceToDeviceList(int dNum,const char *dName) {
	if(dName!=NULL) {
		GtkTreeIter iter;
		gtk_list_store_append(deviceListStore,&iter);
		gtk_list_store_set(deviceListStore,&iter,0,dNum,1,dName,-1);
	}
}

void updateDeviceListEntry(GtkTreeIter iter,char *newName) {
	gtk_list_store_set(deviceListStore,&iter,1,newName,-1);
}

void clearTrackList() {
	gtk_list_store_clear(trackListStore);
}

void updateTrackList(trackDetails *tDetails) {
	if(tDetails!=NULL) {
		GtkTreeIter iter;
		gtk_list_store_append(trackListStore,&iter);
		gtk_list_store_set(trackListStore,&iter,ID,tDetails->id,
												TITLE,tDetails->title,
												ARTIST,tDetails->artist,
												ALBUM,tDetails->album,
												GENRE,tDetails->genre,
												RATING,tDetails->rating,
												-1);
	}
}
