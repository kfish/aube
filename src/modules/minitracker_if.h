#ifndef __MINITRACKER_IF_H__
#define __MINITRACKER_IF_H__

#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include "modulewindow.h"
#include "minitracker.h"

G_BEGIN_DECLS

#define MINITRACKER_IF(obj)          GTK_CHECK_CAST (obj, minitracker_if_get_type (), MinitrackerIF)
#define MINITRACKER_IF_CLASS(klass)  GTK_CHECK_CLASS_CAST (klass, minitracker_if_get_type (), MinitrackerIFClass)
#define IS_MINITRACKER_IF(obj)       GTK_CHECK_TYPE (obj, minitracker_if_get_type ())

typedef struct _MinitrackerIF MinitrackerIF;
typedef struct _MinitrackerIFClass MinitrackerIFClass;

struct _MinitrackerIF {
	ModuleWindow window;

	GtkWidget *tl;
	minitracker *data;
};

struct _MinitrackerIFClass {
	ModuleWindowClass parent_class;

	void (*minitracker_if) (MinitrackerIF * a);
};

GType minitracker_if_get_type(void);
GtkWidget *minitracker_if_new(minitracker * rsq);
void minitracker_if_close_cb(GtkWidget * widget, gpointer data);

G_END_DECLS

#endif /* __MINITRACKER_IF_H__ */
