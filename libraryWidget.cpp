#include "libraryWidget.h"


libraryWidget::libraryWidget() {
	this->listStore = gtk_list_store_new(NUM_COL,
											G_TYPE_STRING,
											G_TYPE_STRING,
											G_TYPE_STRING,
											G_TYPE_STRING,
											G_TYPE_STRING,
											G_TYPE_STRING,
											G_TYPE_STRING,
											G_TYPE_STRING,
											G_TYPE_STRING,
											G_TYPE_STRING,
											G_TYPE_STRING,
											G_TYPE_STRING,
											G_TYPE_STRING);
	
	this->listWidget = gtk_tree_view_new_with_model(GTK_TREE_MODEL(this->listStore));
	
	GtkTreeSelection *select;
	select = gtk_tree_view_get_selection(GTK_TREE_VIEW(this->listWidget));
	gtk_tree_selection_set_mode(select,GTK_SELECTION_SINGLE);
	g_signal_connect(G_OBJECT(select),"changed",G_CALLBACK(this->selected),this);
	
	GtkCellRenderer *renderer;
	
	renderer = gtk_cell_renderer_text_new();
	this->name_column = gtk_tree_view_column_new_with_attributes("Name",renderer,"text",NAME,NULL);
	renderer = gtk_cell_renderer_text_new();
	this->artist_column = gtk_tree_view_column_new_with_attributes("Artist",renderer,"text",ARTIST,NULL);
	renderer = gtk_cell_renderer_text_new();
	this->composer_column = gtk_tree_view_column_new_with_attributes("Composer",renderer,"text",COMPOSER,NULL);
	renderer = gtk_cell_renderer_text_new();
	this->genre_column = gtk_tree_view_column_new_with_attributes("Genre",renderer,"text",GENRE,NULL);
	renderer = gtk_cell_renderer_text_new();
	this->album_column = gtk_tree_view_column_new_with_attributes("Album",renderer,"text",ALBUM,NULL);
	renderer = gtk_cell_renderer_text_new();
	this->date_column = gtk_tree_view_column_new_with_attributes("Date",renderer,"text",DATE,NULL);
	renderer = gtk_cell_renderer_text_new();
	this->track_column = gtk_tree_view_column_new_with_attributes("#",renderer,"text",TRACK_NUM,NULL);
	renderer = gtk_cell_renderer_text_new();
	this->duration_column = gtk_tree_view_column_new_with_attributes("Duration",renderer,"text",DURATION,NULL);
	renderer = gtk_cell_renderer_text_new();
	this->rating_column = gtk_tree_view_column_new_with_attributes("Rating",renderer,"text",RATING,NULL);
		
	this->setColumns();
	this->addItem();
}

void libraryWidget::setColumns() {
	gtk_tree_view_append_column(GTK_TREE_VIEW(this->listWidget),this->name_column);
	gtk_tree_view_append_column(GTK_TREE_VIEW(this->listWidget),this->artist_column);
	gtk_tree_view_append_column(GTK_TREE_VIEW(this->listWidget),this->composer_column);
	gtk_tree_view_append_column(GTK_TREE_VIEW(this->listWidget),this->genre_column);
	gtk_tree_view_append_column(GTK_TREE_VIEW(this->listWidget),this->album_column);
	gtk_tree_view_append_column(GTK_TREE_VIEW(this->listWidget),this->date_column);
	gtk_tree_view_append_column(GTK_TREE_VIEW(this->listWidget),this->track_column);
	gtk_tree_view_append_column(GTK_TREE_VIEW(this->listWidget),this->duration_column);
	gtk_tree_view_append_column(GTK_TREE_VIEW(this->listWidget),this->rating_column);
}

void libraryWidget::hideColumn() {
	gtk_tree_view_column_set_visible(this->album_column,false);
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

void libraryWidget::addItem() {
	GtkTreeIter iter;
	gtk_list_store_append(this->listStore,&iter);
	gtk_list_store_set(this->listStore,&iter,
								ID,"13",
								NAME,"name1",
								ARTIST,"artist1",
								COMPOSER,"composer1",
								GENRE,"genre1",
								ALBUM,"abum1",
								DATE,"date1",
								TRACK_NUM,"track1",
								DURATION,"duration1",
								RATING,"rating1",
								-1);
}
