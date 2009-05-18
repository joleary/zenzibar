#include <gtk/gtk.h>

extern GtkWidget *rootWindow;
extern GtkWidget *rootVBox;
extern GtkWidget *windowMenu;
extern GtkWidget *windowFooter;
extern GtkWidget *mainContainerHPane;
extern GtkWidget *mainSideBarScrollContainer;
extern GtkWidget *sidebarDeviceList;
extern GtkWidget *mainScrollContainer;
extern GtkWidget *mainList;
extern GtkWidget *mainProgressBar;
extern GtkTreeSelection *deviceSelect;
void drawUI();
void arrangeUI();
void showUI();
void addDeviceToDeviceList(int,char *);
void updateDeviceList(GtkTreeIter,char *);
