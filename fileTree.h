#include "fsProvider.h"
#include <string>
#include <gtk/gtk.h>

class fileTree {

	public:
		fileTree();
		GtkWidget *getWidget();
		static void selected(GtkTreeSelection *,fileTree*);
		
	private:
		enum {
			EXPANDED,
			NAME,
			HIDDEN,
			PATH,
			NUM_COLUMNS
		};
		GtkWidget *treeWidget;
		GtkTreeStore *treeStore;
		GtkTreeIter *treeIter;
		fsProvider *fsp;
		void addNode(GtkTreeIter *,bool,const char *,bool,const char *);
		
};
