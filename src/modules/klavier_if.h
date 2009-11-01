#ifndef __KLAVIER_IF_H__
#define __KLAVIER_IF_H__

#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include "klavier.h"

#ifdef __cplusplus
extern "C" {
#endif				/* __cplusplus */


#define KLAVIER_IF(obj)          GTK_CHECK_CAST (obj, klavier_if_get_type (), KlavierIF)
#define KLAVIER_IF_CLASS(klass)  GTK_CHECK_CLASS_CAST (klass, klavier_if_get_type (), KlavierIFClass)
#define IS_KLAVIER_IF(obj)       GTK_CHECK_TYPE (obj, klavier_if_get_type ())


	typedef struct _KlavierIF KlavierIF;
	typedef struct _KlavierIFClass KlavierIFClass;

	struct _KlavierIF {
		GtkWindow window;

		klavier *data;
	};

	struct _KlavierIFClass {
		GtkWindowClass parent_class;

		void (*klavier_if) (KlavierIF * a);
	};

	GType klavier_if_get_type(void);
	GtkWidget *klavier_if_new(klavier * k);
	void klavier_if_close_cb(GtkWidget * widget, gpointer data);

#ifdef __cplusplus
}
#endif				/* __cplusplus */
#endif				/* __KLAVIER_IF_H__ */
