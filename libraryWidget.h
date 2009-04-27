#include <gtk/gtk.h>

class libraryWidget {

	public:
		libraryWidget();
		GtkWidget *getLibraryWidget();
		static void selected(GtkTreeSelection*,libraryWidget*);
	private:
		GtkWidget *listWidget;
		GtkListStore *listStore;
		enum {
			NAME,
			ZENID,
			ALBUM,
			NUM_COL
		};
		void addItem(const char*,const char*,const char*);

};
