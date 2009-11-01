#ifndef __PANNER4_IF_H__
#define __PANNER4_IF_H__

#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include "panner4.h"

#ifdef __cplusplus
extern "C" {
#endif				/* __cplusplus */


#define PANNER4_IF(obj)          GTK_CHECK_CAST (obj, panner4_if_get_type (), Panner4IF)
#define PANNER4_IF_CLASS(klass)  GTK_CHECK_CLASS_CAST (klass, panner4_if_get_type (), Panner4IFClass)
#define IS_PANNER4_IF(obj)       GTK_CHECK_TYPE (obj, panner4_if_get_type ())


	typedef struct _Panner4IF Panner4IF;
	typedef struct _Panner4IFClass Panner4IFClass;

	struct _Panner4IF {
		GtkWindow window;

		panner4 *data;
	};

	struct _Panner4IFClass {
		GtkWindowClass parent_class;

		void (*panner4_if) (Panner4IF * a);
	};

	GType panner4_if_get_type(void);
	GtkWidget *panner4_if_new(panner4 * pn);
	void panner4_if_close_cb(GtkWidget * widget, gpointer data);

#ifdef __cplusplus
}
#endif				/* __cplusplus */
#endif				/* __PANNER4_IF_H__ */
