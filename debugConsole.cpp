#include "debugConsole.h"


void debugConsole::print(std::string message) {
	
}

void debugConsole::print(const char *message) {
	gtk_text_buffer_insert(textBuffer,&iter,message,-1);
	gtk_text_buffer_insert(textBuffer,&iter,"\n",-1);
}

void debugConsole::addToContainer(GtkContainer *container) {
	if(container!=NULL) {
		gtk_container_add(container,this->scrollContainer);
	}
}

debugConsole* debugConsole::Instance() {
	if(debugConsole::pInstance==0) {
		debugConsole::pInstance = new debugConsole();
	}
	return debugConsole::pInstance;
}

debugConsole::debugConsole() {
	scrollContainer = gtk_scrolled_window_new(NULL,NULL);
	textView = gtk_text_view_new();
	gtk_container_add(GTK_CONTAINER(scrollContainer),textView);
	textBuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textView));
	gtk_text_buffer_get_iter_at_offset(textBuffer,&iter,0);
	gtk_text_view_set_editable(GTK_TEXT_VIEW(textView),false);
	gtk_text_view_set_left_margin(GTK_TEXT_VIEW(textView),10);
	gtk_text_view_set_right_margin(GTK_TEXT_VIEW(textView),10);
}

debugConsole* debugConsole::pInstance=0;
