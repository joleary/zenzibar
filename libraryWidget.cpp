#include "libraryWidget.h"


libraryWidget::libraryWidget() {
	this->listStore = gtk_list_store_new(NUM_COL,G_TYPE_STRING,G_TYPE_STRING,G_TYPE_STRING);
	
	this->listWidget = gtk_tree_view_new_with_model(GTK_TREE_MODEL(this->listStore));
	
	GtkTreeSelection *select;
	select = gtk_tree_view_get_selection(GTK_TREE_VIEW(this->listWidget));
	gtk_tree_selection_set_mode(select,GTK_SELECTION_SINGLE);
	g_signal_connect(G_OBJECT(select),"changed",G_CALLBACK(this->selected),this);

	
	GtkCellRenderer *renderer;
	GtkTreeViewColumn *column;
	
	// name column
	renderer = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new_with_attributes("name",renderer,"text",NAME,NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(this->listWidget),column);

	// id column
	renderer = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new_with_attributes("id",renderer,"text",ZENID,NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(this->listWidget),column);
	
	// album column
	renderer = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new_with_attributes("album",renderer,"text",ALBUM,NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(this->listWidget),column);
	
	this->addItem("track1","1","album1");
	this->addItem("track2","2","album2");
	this->addItem("track3","3","album3");
}

void libraryWidget::selected(GtkTreeSelection *treeSelect,libraryWidget *ref) {
	GtkTreeIter iter;
	GtkTreeModel *model;
	char *nameSelected;
	gtk_tree_selection_get_selected(treeSelect, &model, &iter);
	gtk_tree_model_get(model,&iter,NAME,&nameSelected,-1);
}

GtkWidget* libraryWidget::getLibraryWidget() {
	return this->listWidget;
}

void libraryWidget::addItem(const char *name,const char *id,const char *album) {
	GtkTreeIter iter;
	gtk_list_store_append(this->listStore,&iter);
	gtk_list_store_set(this->listStore,&iter,NAME,name,ZENID,id,ALBUM,album,-1);
}
