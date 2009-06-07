#include "gtkzenui.h"

GtkWidget *rootWindow;
GtkWidget *rootVBox;
GtkWidget *windowMenu;
GtkWidget *windowFooter;
GtkWidget *mainContainerHPane;
GtkWidget *mainProgressBar;
GtkWidget *mainTabContainer;

GtkWidget *musicLibraryList;
GtkListStore *musicLibraryListStore;
GtkTreeViewColumn *mlTitleColumn;
GtkTreeViewColumn *mlArtistColumn;
GtkTreeViewColumn *mlAlbumColumn;
GtkTreeViewColumn *mlGenreColumn;
GtkTreeViewColumn *mlDurationColumn;
GtkTreeViewColumn *mlRatingColumn;
GtkTreeSelection *mlTrackSelect;
GtkWidget *musicLibraryScrollContainer;

GtkWidget *sidebarDeviceList;
GtkTreeStore *deviceTreeStore;
GtkTreeViewColumn *deviceColumn;
GtkTreeSelection *deviceSelect;
GtkWidget *mainSideBarScrollContainer;

GtkWidget *trackList;
GtkListStore *trackListStore;
GtkTreeViewColumn *titleColumn;
GtkTreeViewColumn *artistColumn;
GtkTreeViewColumn *albumColumn;
GtkTreeViewColumn *genreColumn;
GtkTreeViewColumn *durationColumn;
GtkTreeViewColumn *ratingColumn;
GtkTreeSelection *trackSelect;
GtkWidget *deviceTrackListScrollContainer;

enum {ID,TITLE,ARTIST,ALBUM,GENRE,DURATION,RATING,TRACK_COLUMNS};

/*
 * Function prototypes
 */

void buildMusicLibraryWidget();

void buildDeviceListWidget();

void buildDeviceTrackListWidget();


void drawUI() {
	rootWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	rootVBox = gtk_vbox_new(FALSE,0);
	windowMenu = gtk_menu_bar_new();
	windowFooter = gtk_statusbar_new();
	mainContainerHPane = gtk_hpaned_new();
	mainTabContainer = gtk_notebook_new();
	gtk_paned_set_position(GTK_PANED(mainContainerHPane),140);
	mainProgressBar = gtk_progress_bar_new();
	buildDeviceListWidget();
	buildMusicLibraryWidget();
	buildDeviceTrackListWidget();
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
	gtk_paned_add2(GTK_PANED(mainContainerHPane),mainTabContainer);
	gtk_notebook_append_page(GTK_NOTEBOOK(mainTabContainer),deviceTrackListScrollContainer,NULL);
	gtk_notebook_append_page(GTK_NOTEBOOK(mainTabContainer),musicLibraryScrollContainer,NULL);
}

void showUI() {
	GdkScreen *screen = gtk_window_get_screen(GTK_WINDOW(rootWindow));
	gtk_window_set_default_size(GTK_WINDOW(rootWindow),gdk_screen_get_width(screen)/2,gdk_screen_get_height(screen)/2);
	gtk_widget_show_all(rootWindow);
	gtk_widget_hide(mainProgressBar);
}


void addDeviceNode(GtkTreeIter *,folderTree *);

void populateDeviceTree(folderTree *root) {
	if(root) {
		GtkTreeIter iter;
		gtk_tree_store_append(deviceTreeStore,&iter,NULL);
		gtk_tree_store_set(deviceTreeStore,&iter,0,root->uid,1,root->name,-1);
		if(root->child) {
			addDeviceNode(&iter,root->child);
		}
	}
}

void addDeviceNode(GtkTreeIter *parentIter,folderTree *node) {
	GtkTreeIter childIter;
	gtk_tree_store_append(deviceTreeStore,&childIter,parentIter);
	gtk_tree_store_set(deviceTreeStore,&childIter,0,node->uid,1,node->name,-1);
	if(node->child) {
		addDeviceNode(&childIter,node->child);
	}
	if(node->sibling) {
		addDeviceNode(parentIter,node->sibling);
	}
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

void statusBarLog(const char *message) {
	fprintf(stdout,"DEBUG: %s\n",message);
	guint contextId;
	contextId = gtk_statusbar_get_context_id(GTK_STATUSBAR(windowFooter),"statusMessage");
	gtk_statusbar_push(GTK_STATUSBAR(windowFooter),contextId,message);
}

void buildMusicLibraryWidget() {
	
	musicLibraryListStore = gtk_list_store_new(TRACK_COLUMNS, G_TYPE_INT,
														G_TYPE_STRING,
														G_TYPE_STRING,
														G_TYPE_STRING,
														G_TYPE_STRING,
														G_TYPE_STRING,
														G_TYPE_INT);
	musicLibraryList = gtk_tree_view_new_with_model(GTK_TREE_MODEL(musicLibraryListStore));
	
	mlTitleColumn = gtk_tree_view_column_new_with_attributes("title",gtk_cell_renderer_text_new(),"text",TITLE,NULL);
	gtk_tree_view_column_set_sort_column_id(mlTitleColumn,TITLE);
	mlArtistColumn = gtk_tree_view_column_new_with_attributes("artist",gtk_cell_renderer_text_new(),"text",ARTIST,NULL);
	gtk_tree_view_column_set_sort_column_id(mlArtistColumn,ARTIST);
	mlAlbumColumn = gtk_tree_view_column_new_with_attributes("album",gtk_cell_renderer_text_new(),"text",ALBUM,NULL);
	gtk_tree_view_column_set_sort_column_id(mlAlbumColumn,ALBUM);
	mlGenreColumn = gtk_tree_view_column_new_with_attributes("genre",gtk_cell_renderer_text_new(),"text",GENRE,NULL);
	gtk_tree_view_column_set_sort_column_id(mlGenreColumn,GENRE);
	mlDurationColumn = gtk_tree_view_column_new_with_attributes("duration",gtk_cell_renderer_text_new(),"text",DURATION,NULL);
	gtk_tree_view_column_set_sort_column_id(mlDurationColumn,DURATION);
	mlRatingColumn = gtk_tree_view_column_new_with_attributes("rating",gtk_cell_renderer_text_new(),"text",RATING,NULL);
	gtk_tree_view_column_set_sort_column_id(mlRatingColumn,RATING);
	
	gtk_tree_view_append_column(GTK_TREE_VIEW(musicLibraryList),mlTitleColumn);
	gtk_tree_view_append_column(GTK_TREE_VIEW(musicLibraryList),mlArtistColumn);
	gtk_tree_view_append_column(GTK_TREE_VIEW(musicLibraryList),mlAlbumColumn);
	gtk_tree_view_append_column(GTK_TREE_VIEW(musicLibraryList),mlGenreColumn);
	gtk_tree_view_append_column(GTK_TREE_VIEW(musicLibraryList),mlDurationColumn);
	gtk_tree_view_append_column(GTK_TREE_VIEW(musicLibraryList),mlRatingColumn);
	mlTrackSelect = gtk_tree_view_get_selection(GTK_TREE_VIEW(musicLibraryList));
	gtk_tree_selection_set_mode(mlTrackSelect, GTK_SELECTION_SINGLE);
	
	musicLibraryScrollContainer = gtk_scrolled_window_new(NULL,NULL);
	gtk_container_add(GTK_CONTAINER(musicLibraryScrollContainer),musicLibraryList);
}

void buildDeviceListWidget() {
	deviceTreeStore = gtk_tree_store_new(2, G_TYPE_INT, G_TYPE_STRING);
	sidebarDeviceList = gtk_tree_view_new_with_model(GTK_TREE_MODEL(deviceTreeStore));
	deviceColumn = gtk_tree_view_column_new_with_attributes("Devices", gtk_cell_renderer_text_new(), "text", 1, NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(sidebarDeviceList), deviceColumn);
	deviceSelect = gtk_tree_view_get_selection(GTK_TREE_VIEW(sidebarDeviceList));
	gtk_tree_selection_set_mode(deviceSelect, GTK_SELECTION_SINGLE);
	
	mainSideBarScrollContainer = gtk_scrolled_window_new(NULL, NULL);
	gtk_container_add(GTK_CONTAINER(mainSideBarScrollContainer),sidebarDeviceList);
}

void buildDeviceTrackListWidget() {
	trackListStore = gtk_list_store_new(TRACK_COLUMNS, G_TYPE_INT,
														G_TYPE_STRING,
														G_TYPE_STRING,
														G_TYPE_STRING,
														G_TYPE_STRING,
														G_TYPE_STRING,
														G_TYPE_INT);
	trackList = gtk_tree_view_new_with_model(GTK_TREE_MODEL(trackListStore));
	titleColumn = gtk_tree_view_column_new_with_attributes("title",gtk_cell_renderer_text_new(),"text",TITLE,NULL);
	gtk_tree_view_column_set_sort_column_id(titleColumn,TITLE);
	artistColumn = gtk_tree_view_column_new_with_attributes("artist",gtk_cell_renderer_text_new(),"text",ARTIST,NULL);
	gtk_tree_view_column_set_sort_column_id(artistColumn,ARTIST);
	albumColumn = gtk_tree_view_column_new_with_attributes("album",gtk_cell_renderer_text_new(),"text",ALBUM,NULL);
	gtk_tree_view_column_set_sort_column_id(albumColumn,ALBUM);
	genreColumn = gtk_tree_view_column_new_with_attributes("genre",gtk_cell_renderer_text_new(),"text",GENRE,NULL);
	gtk_tree_view_column_set_sort_column_id(genreColumn,GENRE);
	durationColumn = gtk_tree_view_column_new_with_attributes("duration",gtk_cell_renderer_text_new(),"text",DURATION,NULL);
	gtk_tree_view_column_set_sort_column_id(durationColumn,DURATION);
	ratingColumn = gtk_tree_view_column_new_with_attributes("rating",gtk_cell_renderer_text_new(),"text",RATING,NULL);
	gtk_tree_view_column_set_sort_column_id(ratingColumn,RATING);
	gtk_tree_view_append_column(GTK_TREE_VIEW(trackList),titleColumn);
	gtk_tree_view_append_column(GTK_TREE_VIEW(trackList),artistColumn);
	gtk_tree_view_append_column(GTK_TREE_VIEW(trackList),albumColumn);
	gtk_tree_view_append_column(GTK_TREE_VIEW(trackList),genreColumn);
	gtk_tree_view_append_column(GTK_TREE_VIEW(trackList),durationColumn);
	gtk_tree_view_append_column(GTK_TREE_VIEW(trackList),ratingColumn);
	trackSelect = gtk_tree_view_get_selection(GTK_TREE_VIEW(trackList));
	gtk_tree_selection_set_mode(trackSelect, GTK_SELECTION_SINGLE);
	
	deviceTrackListScrollContainer = gtk_scrolled_window_new(NULL,NULL);
	gtk_container_add(GTK_CONTAINER(deviceTrackListScrollContainer),trackList);
}
