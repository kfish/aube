#ifndef __MINIMAUBE_IF_H__
#define __MINIMAUBE_IF_H__

#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include "modulewindow.h"
#include "minimaube.h"

G_BEGIN_DECLS

#define MINIMAUBE_IF(obj)          GTK_CHECK_CAST (obj, minimaube_if_get_type (), MiniMaubeIF)
#define MINIMAUBE_IF_CLASS(klass)  GTK_CHECK_CLASS_CAST (klass, minimaube_if_get_type (), MiniMaubeIFClass)
#define IS_MINIMAUBE_IF(obj)       GTK_CHECK_TYPE (obj, minimaube_if_get_type ())


typedef struct _MiniMaubeIF MiniMaubeIF;
typedef struct _MiniMaubeIFClass MiniMaubeIFClass;

struct _MiniMaubeIF {
	ModuleWindow window;

	GtkWidget *replace_menuitem;
	GtkWidget *vbox;
	GtkWidget *hboxes[MAX_CHANNELS];
	minimaube *data;
};

struct _MiniMaubeIFClass {
	ModuleWindowClass parent_class;

	void (*minimaube_if) (MiniMaubeIF * a);
};

GType minimaube_if_get_type(void);
GtkWidget *minimaube_if_new(minimaube * dl);
void minimaube_if_close_cb(GtkWidget * widget, gpointer data);

G_END_DECLS

#endif /* __MINIMAUBE_IF_H__ */
