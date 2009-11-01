#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>
#include "modulewindow.h"

extern GtkWidget *master_daddy;
extern aube_data *aube_daddy;

static void modulewindow_class_init(ModuleWindowClass * klass);
static void modulewindow_init(ModuleWindow * b);
GtkWidget *modulewindow_new();

GType modulewindow_get_type(void)
{
	static GType mw_type = 0;

	if (!mw_type) {
		static const GTypeInfo mw_info = {
			sizeof(ModuleWindowClass),
			NULL,	/* base_init */
			NULL,	/* base_finalise */
			(GClassInitFunc) modulewindow_class_init,
			NULL,	/* class_finalize */
			NULL,	/* class_data */
			sizeof(ModuleWindow),
			0,	/* n_preallocs */
			(GInstanceInitFunc) modulewindow_init,
		};

		mw_type = g_type_register_static(GTK_TYPE_WINDOW,
                        "ModuleWindow", &mw_info, 0);
	}
	return mw_type;
}

static void modulewindow_class_init(ModuleWindowClass * klass)
{
}

static void
modulewindow_close_cb(GtkAccelGroup * accel_group,
		GObject * acceleratable,
		guint keyval, GdkModifierType modifier)
{
	if (GTK_IS_WINDOW(acceleratable))
		g_signal_emit_by_name(G_OBJECT(acceleratable), "destroy");
}


static void modulewindow_init(ModuleWindow * mw)
{
	GClosure *gclosure;
	GtkAccelGroup *accel_group;

        /*
         * close the window when Ctrl and w are pressed.
         *      accel key combo is static. perhaps there is a better
         *      way to do this?
         */
	accel_group = gtk_accel_group_new();
	gclosure = g_cclosure_new((GCallback)modulewindow_close_cb, NULL, NULL);
	gtk_accel_group_connect(accel_group, GDK_w, GDK_CONTROL_MASK, 0, gclosure);
	gtk_window_add_accel_group(GTK_WINDOW(mw), accel_group);
}

#if 0
static void modulewindow_dismiss (GtkWidget * widget, gpointer data)
{
	gtk_widget_destroy(GTK_WIDGET(data));
}
#endif

GtkWidget *modulewindow_new(module * module)
{
        return GTK_WIDGET ( g_object_new (MODULEWINDOW_TYPE, NULL));
}