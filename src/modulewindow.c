#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
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

static void modulewindow_init(ModuleWindow * a)
{
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
