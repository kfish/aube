
#ifndef __NONE_CONFIG_H__
#define __NONE_CONFIG_H__

#include <gdk/gdk.h>
#include <gtk/gtk.h>

#include "aube.h"

#ifdef __cplusplus
extern "C" {
#endif				/*
				   __cplusplus 
				 */


#define NONE_CONFIG(obj)          GTK_CHECK_CAST (obj, none_config_get_type (), NoneConfig)
#define NONE_CONFIG_CLASS(klass)  GTK_CHECK_CLASS_CAST (klass, none_config_get_type (), NoneConfigClass)
#define IS_NONE_CONFIG(obj)       GTK_CHECK_TYPE (obj, none_config_get_type ())


	typedef struct _NoneConfig NoneConfig;
	typedef struct _NoneConfigClass NoneConfigClass;

	struct _NoneConfig {
		GtkWindow window;
	};

	struct _NoneConfigClass {
		GtkWindowClass parent_class;

		void (*none_config) (NoneConfig * a);
	};

	void none_config_cb(module * u);

	GType none_config_get_type(void);
	GtkWidget *none_config_new(module * u);

#ifdef __cplusplus
}
#endif				/*
				   __cplusplus 
				 */
#endif				/*
				   __NONE_CONFIG_H__ 
				 */
