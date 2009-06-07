#include <glib-2.0/glib.h>
#include <glib-2.0/glib-object.h>


#define THREAD_TYPE_MANAGER                  (thread_manager_get_type ())
#define THREAD_MANAGER(obj)                  (G_TYPE_CHECK_INSTANCE_CAST ((obj), THREAD_TYPE_MANAGER, ThreadManager))
#define THREAD_IS_MANAGER(obj)               (G_TYPE_CHECK_INSTANCE_TYPE ((obj), THREAD_TYPE_MANAGER))
#define THREAD_MANAGER_CLASS(klass)          (G_TYPE_CHECK_CLASS_CAST ((klass), THREAD_TYPE_MANAGER, ThreadManagerClass))
#define THREAD_IS_MANAGER_CLASS(klass)       (G_TYPE_CHECK_CLASS_TYPE ((klass), THREAD_TYPE_MANAGER))
#define THREAD_MANAGER_GET_CLASS(obj)        (G_TYPE_INSTANCE_GET_CLASS ((obj), THREAD_TYPE_MANAGER, ThreadManagerClass))

#define THREAD_TYPE_MANAGER (thread_manager_get_type ())

enum {
	PROP_0,
	PROP_THREAD_STATUS
};

typedef struct {
  GObject parent;
  gchar *status;
} ThreadManager;

typedef struct {
  GObjectClass parent;
} ThreadManagerClass;

typedef void (*cmarshal_NONE) (GObject *object,gpointer *data);
