#include "threadManager.h"

static void thread_manager_class_init(ThreadManagerClass *);

GType thread_manager_get_type (void) {
	static GType type = 0;
	if (type == 0) {
		static const GTypeInfo info = {
			sizeof (ThreadManagerClass), /* class size */
			NULL,           /* base_init */
			NULL,           /* base_finalize */
			(GClassInitFunc) thread_manager_class_init,
			NULL,           /* class_finalize */
			NULL,           /* class_data */
			sizeof (ThreadManager), /* object size */
			0,              /* n_preallocs */
			(GInstanceInitFunc) NULL /* instance_init */
		};
		type = g_type_register_static (G_TYPE_OBJECT, "ManagerType", &info, 0);
	}
  return type;
}

static void thread_manager_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec) {
	ThreadManager *self = THREAD_MANAGER (object);

	switch (property_id) {
		case PROP_THREAD_STATUS:
			//g_free (self->priv->status);
			self->status = g_value_dup_string (value);
			g_print ("status: %s\n", self->status);
			g_signal_emit_by_name(self,"status");
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
			break;
	}
}

static void thread_manager_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec) {
	ThreadManager *self = THREAD_MANAGER (object);

	switch (property_id) {
		case PROP_THREAD_STATUS:
			g_value_set_string (value, self->status);
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
			break;
	}
}

static void thread_manager_class_init(ThreadManagerClass *klass) {
	GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
	GParamSpec *pspec;

	gobject_class->set_property = thread_manager_set_property;
	gobject_class->get_property = thread_manager_get_property;

	pspec = g_param_spec_string ("status", "manager construct property", 
									"get / set manager status", "unknown", G_PARAM_READWRITE);
	g_object_class_install_property (gobject_class, PROP_THREAD_STATUS, pspec);
	g_signal_new("status",THREAD_TYPE_MANAGER,G_SIGNAL_RUN_LAST,0,NULL,NULL,g_cclosure_marshal_VOID__VOID,G_TYPE_NONE,0,NULL);
}
