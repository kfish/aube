#ifndef __FILTERRESLP_IF_H__
#define __FILTERRESLP_IF_H__

#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include "filter_reslp.h"

#ifdef __cplusplus
extern "C" {
#endif				/* __cplusplus */


#define FILTERRESLP_IF(obj)          GTK_CHECK_CAST (obj, filter_reslp_if_get_type (), FilterResLP_IF)
#define FILTERRESLP_IF_CLASS(klass)  GTK_CHECK_CLASS_CAST (klass, filter_reslp_if_get_type (), FilterResLP_IFClass)
#define IS_FILTERRESLP_IF(obj)       GTK_CHECK_TYPE (obj, filter_reslp_if_get_type ())


	typedef struct _FilterResLP_IF FilterResLP_IF;
	typedef struct _FilterResLP_IFClass FilterResLP_IFClass;

	struct _FilterResLP_IF {
		GtkWindow window;

		GtkWidget *resonance_env_curve;
		GtkWidget *cutoff_env_curve;

		gint env_tag;
		filter_reslp *data;
	};

	struct _FilterResLP_IFClass {
		GtkWindowClass parent_class;

		void (*filter_reslp_if) (FilterResLP_IF * a);
	};

	GType filter_reslp_if_get_type(void);
	GtkWidget *filter_reslp_if_new(filter_reslp * fr);
	void filter_reslp_if_close_cb(GtkWidget * widget, gpointer data);

#ifdef __cplusplus
}
#endif				/* __cplusplus */
#endif				/* __FILTERRESLP_IF_H__ */
