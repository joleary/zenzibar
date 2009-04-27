#include "fileTree.h"

fileTree::fileTree() {
	this->fsp = fsProvider::Instance();
	
	this->treeStore = gtk_tree_store_new(NUM_COLUMNS,G_TYPE_BOOLEAN,G_TYPE_STRING,G_TYPE_BOOLEAN,G_TYPE_STRING);
	
	this->treeWidget = gtk_tree_view_new_with_model(GTK_TREE_MODEL(this->treeStore));
	
	GtkTreeSelection *select;
	select = gtk_tree_view_get_selection(GTK_TREE_VIEW(this->treeWidget));
	gtk_tree_selection_set_mode(select,GTK_SELECTION_SINGLE);
	g_signal_connect(G_OBJECT(select),"changed",G_CALLBACK(this->selected),this);

	
	GtkCellRenderer *renderer;
	GtkTreeViewColumn *column;

	// expanded column
	//renderer = gtk_cell_renderer_toggle_new();
	//column = gtk_tree_view_column_new_with_attributes("expanded",renderer,"toggle",EXPANDED,NULL);
	//gtk_tree_view_append_column(GTK_TREE_VIEW(this->treeWidget),column);
	
	// name column
	renderer = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new_with_attributes("name",renderer,"text",NAME,NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(this->treeWidget),column);

	// hidden column
	//renderer = gtk_cell_renderer_toggle_new();
	//column = gtk_tree_view_column_new_with_attributes("hidden",renderer,"toggle",HIDDEN,NULL);
	//gtk_tree_view_append_column(GTK_TREE_VIEW(this->treeWidget),column);
	
	// path column
	renderer = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new_with_attributes("path",renderer,"text",PATH,NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(this->treeWidget),column);

	GtkTreeIter iter;
	
	
	this->addNode(NULL,false,"Home",true,"/home/joleary");	
	this->addNode(NULL,false,"Root",true,"/");
}

void fileTree::selected(GtkTreeSelection *selected,fileTree *data) {
	fsProvider *fileProvider = fsProvider::Instance();
	GtkTreeIter iter;
	GtkTreeModel *model;
	char *pathSelected;
	gtk_tree_selection_get_selected(selected, &model, &iter);
	gtk_tree_model_get(model,&iter,PATH,&pathSelected,-1);
	GFileEnumerator *fileIter = fileProvider->getChildren(g_file_new_for_path(pathSelected));
	GFileInfo *finfo = g_file_enumerator_next_file(fileIter,NULL,NULL);
	while(finfo!=NULL) {
		if(!fileProvider->isHidden(finfo)) {
			std::string fullPath=pathSelected;
			fullPath.append("/");
			fullPath.append(fileProvider->getName(finfo));
			data->addNode(&iter,false,fileProvider->getName(finfo),
										fileProvider->isHidden(finfo),
											fullPath.c_str());
		}
		finfo = g_file_enumerator_next_file(fileIter,NULL,NULL);	
	}
	
	g_object_unref(fileIter);
}

GtkWidget* fileTree::getWidget() {
	return this->treeWidget;
}

void fileTree::addNode(GtkTreeIter *iter,bool toggled,const char *name,bool hidden,const char *path) {
	GtkTreeIter childIter;
	gtk_tree_store_append(this->treeStore,&childIter,iter);
	gtk_tree_store_set(this->treeStore,&childIter,EXPANDED,toggled,NAME,name,HIDDEN,hidden,PATH,path,-1);
}
