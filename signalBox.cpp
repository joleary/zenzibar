#include "signalBox.h"
#include "debugConsole.h"

void signalBox::connect(GtkWidget *widget, gpointer data) {
	debugConsole *console = debugConsole::Instance();
	mtpAdapter *adapter = mtpAdapter::Instance();
	console->print("connecting...");
	gboolean conStatus = adapter->connect();
	if(conStatus) {
		console->print("Connected");
		console->print(adapter->getDeviceDetailsString());
	} else {
		console->print("Failed to connect");
	}
}

void signalBox::disconnect(GtkWidget *widget, gpointer data) {
	debugConsole *console = debugConsole::Instance();
	mtpAdapter *adapter = mtpAdapter::Instance();
	console->print("disconnecting...");
	adapter->disconnect();
	gboolean conStatus = adapter->isConnected();
	if(conStatus) {
		console->print("failed to disconnect");
	} else {
		console->print("disconnected");
	}
}

signalBox* signalBox::Instance() {
	if(signalBox::pInstance==0) {
		signalBox::pInstance = new signalBox();
	}
	return signalBox::pInstance;
}

signalBox::signalBox() {
}

signalBox* signalBox::pInstance = 0;
