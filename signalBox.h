#include <gtk/gtk.h>
#include "mtpAdapter.h"
#include "debugConsole.h"

/*
 * static methods for passing on instructions from the UI to
 * the mtp backend adapter
 */

class signalBox {
	public:
		static signalBox* Instance();
		static void connect(GtkWidget *, gpointer);
		static void disconnect(GtkWidget *,gpointer);

	private:
		static signalBox* pInstance;
		
	protected:
		signalBox();
		signalBox(const signalBox&);
		signalBox& operator= (const signalBox&);

};
