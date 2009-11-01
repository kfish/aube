#ifndef __SYRE_IF_H__
#define __SYRE_IF_H__

#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include "modulewindow.h"
#include "syre.h"

G_BEGIN_DECLS

#define SYRE_IF(obj)          GTK_CHECK_CAST (obj, syre_if_get_type (), SyreIF)
#define SYRE_IF_CLASS(klass)  GTK_CHECK_CLASS_CAST (klass, syre_if_get_type (), SyreIFClass)
#define IS_SYRE_IF(obj)       GTK_CHECK_TYPE (obj, syre_if_get_type ())

typedef struct _SyreIF SyreIF;
typedef struct _SyreIFClass SyreIFClass;

struct _SyreIF {
	ModuleWindow window;

	GtkWidget *sliders[NUM_HARMONICS];
	GtkWidget *amp_env_curve;
	GtkWidget *pitch_env_curve;
	GtkWidget *amp_env_curve_accented;
	GtkWidget *pitch_env_curve_accented;

	gint env_tag;
	syre_if_data *data;
};

struct _SyreIFClass {
	ModuleWindowClass parent_class;

	void (*syre_if) (SyreIF * a);
};

GType syre_if_get_type(void);
GtkWidget *syre_if_new(syre_if_data * sd);
void syre_if_close_cb(GtkWidget * widget, gpointer data);

G_END_DECLS

#endif /* __SYRE_IF_H__ */
