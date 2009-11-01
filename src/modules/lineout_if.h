#ifndef __LINEOUT_IF_H__
#define __LINEOUT_IF_H__

#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include "modulewindow.h"
#include "line.h"

G_BEGIN_DECLS

#define LINEOUT_IF(obj)          GTK_CHECK_CAST (obj, lineout_if_get_type (), LINEOUTIF)
#define LINEOUT_IF_CLASS(klass)  GTK_CHECK_CLASS_CAST (klass, lineout_if_get_type (), LINEOUTIFClass)
#define IS_LINEOUT_IF(obj)       GTK_CHECK_TYPE (obj, lineout_if_get_type ())


typedef struct _LINEOUTIF LINEOUTIF;
typedef struct _LINEOUTIFClass LINEOUTIFClass;

struct _LINEOUTIF {
	ModuleWindow window;

	GtkWidget *inputs_om;
	GtkWidget *inputsmenu;
	oss_out *data;
};

struct _LINEOUTIFClass {
	ModuleWindowClass parent_class;

	void (*lineout_if) (LINEOUTIF * a);
};

GType lineout_if_get_type(void);
GtkWidget *lineout_if_new(oss_out * oss_p);
void lineout_if_close_cb(GtkWidget * widget, gpointer data);

G_END_DECLS

#endif /* __LINEOUT_IF_H__ */
