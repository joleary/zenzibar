#include "gtkzenui.h"

GtkWidget *rootWindow;
GtkWidget *rootVBox;
GtkWidget *windowMenu;
GtkWidget *windowFooter;
GtkWidget *mainContainerHPane;
GtkWidget *mainSideBarScrollContainer;
GtkWidget *sidebarDeviceList;
GtkWidget *mainScrollContainer;
GtkWidget *mainList;
GtkListStore *deviceListStore;
GtkCellRenderer *deviceRenderer;
GtkTreeViewColumn *deviceColumn;
GtkTreeSelection *deviceSelect;
GtkWidget *mainProgressBar;

void drawUI() {
	rootWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	rootVBox = gtk_vbox_new(FALSE,0);
	windowMenu = gtk_menu_bar_new();
	windowFooter = gtk_statusbar_new();
	mainContainerHPane = gtk_hpaned_new();
	mainSideBarScrollContainer = gtk_scrolled_window_new(NULL,NULL);
	
	deviceListStore = gtk_list_store_new(2,G_TYPE_INT,G_TYPE_STRING);
	sidebarDeviceList = gtk_tree_view_new_with_model(GTK_TREE_MODEL(deviceListStore));
	deviceRenderer = gtk_cell_renderer_text_new();
	deviceColumn = gtk_tree_view_column_new_with_attributes("Devices",deviceRenderer,"text",1,NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(sidebarDeviceList),deviceColumn);
	deviceSelect = gtk_tree_view_get_selection(GTK_TREE_VIEW(sidebarDeviceList));
	gtk_tree_selection_set_mode(deviceSelect,GTK_SELECTION_SINGLE);
	
	mainScrollContainer = gtk_scrolled_window_new(NULL,NULL);
	mainList = gtk_tree_view_new();
	mainProgressBar = gtk_progress_bar_new();
	arrangeUI();
	showUI();
}

void arrangeUI() {
	gtk_container_add(GTK_CONTAINER(rootWindow),rootVBox);
	gtk_box_pack_start(GTK_BOX(rootVBox),windowMenu,FALSE,FALSE,0);
	gtk_box_pack_start(GTK_BOX(rootVBox),mainContainerHPane,TRUE,TRUE,0);
	gtk_box_pack_start(GTK_BOX(rootVBox),mainProgressBar,FALSE,FALSE,0);
	gtk_paned_add1(GTK_PANED(mainContainerHPane),mainSideBarScrollContainer);
	gtk_paned_add2(GTK_PANED(mainContainerHPane),mainScrollContainer);
	gtk_container_add(GTK_CONTAINER(mainSideBarScrollContainer),sidebarDeviceList);
	gtk_container_add(GTK_CONTAINER(mainScrollContainer),mainList);
}

void showUI() {
	gtk_widget_show_all(rootWindow);
}

void addDeviceToDeviceList(int dNum,char *dName) {
	if(dName!=NULL) {
		GtkTreeIter iter;
		gtk_list_store_append(deviceListStore,&iter);
		gtk_list_store_set(deviceListStore,&iter,0,dNum,1,dName,-1);
	}
}
