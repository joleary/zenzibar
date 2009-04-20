#include <gtk/gtk.h>
#include <string>

class debugConsole {
	public:
		void print(std::string);
		void print(const char *);
		debugConsole(GtkContainer *);
	private:
		GtkWidget *scrollContainer;
		GtkWidget *textView;
		GtkTextBuffer *textBuffer;
		GtkTextIter iter;
};
