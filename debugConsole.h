#ifndef _DEBUG_CONSOLE
#define _DEBUG_CONSOLE

#include <gtk/gtk.h>
#include <string>

class debugConsole {
	public:
		void print(std::string);
		void print(const char *);
		void addToContainer(GtkContainer *);
		static debugConsole* Instance();
	private:
		static debugConsole* pInstance;
		GtkWidget *scrollContainer;
		GtkWidget *textView;
		GtkTextBuffer *textBuffer;
		GtkTextIter iter;
	protected:
		debugConsole();
};

#endif /* debugConsole.h */
