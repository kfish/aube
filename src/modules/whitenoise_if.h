#ifndef __WHITENOISE_IF_H__
#define __WHITENOISE_IF_H__

#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include "modulewindow.h"
#include "whitenoise.h"

G_BEGIN_DECLS

#define WHITENOISE_IF(obj)          GTK_CHECK_CAST (obj, whitenoise_if_get_type (), WhitenoiseIF)
#define WHITENOISE_IF_CLASS(klass)  GTK_CHECK_CLASS_CAST (klass, whitenoise_if_get_type (), WhitenoiseIFClass)
#define IS_WHITENOISE_IF(obj)       GTK_CHECK_TYPE (obj, whitenoise_if_get_type ())

typedef struct _WhitenoiseIF WhitenoiseIF;
typedef struct _WhitenoiseIFClass WhitenoiseIFClass;

struct _WhitenoiseIF {
	ModuleWindow window;

	whitenoise *data;
};

struct _WhitenoiseIFClass {
	ModuleWindowClass parent_class;

	void (*whitenoise_if) (WhitenoiseIF * a);
};

GType whitenoise_if_get_type(void);
GtkWidget *whitenoise_if_new(whitenoise * wn);
void whitenoise_if_close_cb(GtkWidget * widget, gpointer data);

G_END_DECLS

#endif /* __WHITENOISE_IF_H__ */
