#include <gtk/gtk.h>
#include "handyStructures.h"
extern GtkWidget *rootWindow;
extern GtkWidget *rootVBox;
extern GtkWidget *windowMenu;
extern GtkWidget *windowFooter;
extern GtkWidget *mainContainerHPane;
extern GtkWidget *mainSideBarScrollContainer;
extern GtkWidget *sidebarDeviceList;
extern GtkWidget *mainScrollContainer;
extern GtkWidget *trackList;
extern GtkWidget *mainProgressBar;
extern GtkTreeSelection *deviceSelect;

void drawUI();
void arrangeUI();
void showUI();
void addDeviceToDeviceList(int,const char *);
void updateDeviceListEntry(GtkTreeIter,char *);
void updateTrackList(trackDetails *tDetails);
void clearTrackList();
