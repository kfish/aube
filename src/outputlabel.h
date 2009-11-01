#ifndef __OUTPUTLABEL_H__
#define __OUTPUTLABEL_H__

#include <gdk/gdk.h>
#include <gtk/gtkframe.h>
#include "aube.h"

#ifdef __cplusplus
extern "C" {
#endif				/*
				   __cplusplus 
				 */


#define OUTPUTLABEL(obj)          GTK_CHECK_CAST (obj, outputlabel_get_type (), Outputlabel)
#define OUTPUTLABEL_CLASS(klass)  GTK_CHECK_CLASS_CAST (klass, outputlabel_get_type (), OutputlabelClass)
#define IS_OUTPUTLABEL(obj)       GTK_CHECK_TYPE (obj, outputlabel_get_type ())


	typedef struct _Outputlabel Outputlabel;
	typedef struct _OutputlabelClass OutputlabelClass;

	struct _Outputlabel {
		GtkFrame frame;

		module *module;
		int output_i;
		guint handler_id;
		guint timeout_tag;
		GtkWidget *levelmeter_left, *levelmeter_right;
	};

	struct _OutputlabelClass {
		GtkWindowClass parent_class;

		void (*outputlabel) (Outputlabel * a);
	};

	GType outputlabel_get_type(void);
	GtkWidget *outputlabel_new(module * module, int output_i);


#ifdef __cplusplus
}
#endif				/*
				   __cplusplus 
				 */
#endif				/*
				   __OUTPUTLABEL_H__ 
				 */
