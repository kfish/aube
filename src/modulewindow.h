#ifndef __MODULEWINDOW_H__
#define __MODULEWINDOW_H__

#include <gdk/gdk.h>
#include <gtk/gtkwindow.h>
#include "aube.h"

#ifdef __cplusplus
extern "C" {
#endif	/* __cplusplus */


#define MODULEWINDOW_TYPE          (modulewindow_get_type ())
#define MODULEWINDOW(obj)          GTK_CHECK_CAST (obj, modulewindow_get_type (), ModuleWindow)
#define MODULEWINDOW_CLASS(klass)  GTK_CHECK_CLASS_CAST (klass, modulewindow_get_type (), ModuleWindowClass)
#define IS_MODULEWINDOW(obj)       GTK_CHECK_TYPE (obj, modulewindow_get_type ())


	typedef struct _ModuleWindow ModuleWindow;
	typedef struct _ModuleWindowClass ModuleWindowClass;

	struct _ModuleWindow {
		GtkWindow window;
	};

	struct _ModuleWindowClass {
		GtkWindowClass parent_class;

		void (*modulewindow) (ModuleWindow * a);
	};

	GType modulewindow_get_type(void);
	GtkWidget *modulewindow_new(module * module);


#ifdef __cplusplus
}
#endif	/* __cplusplus */

#endif	/* __MODULEWINDOW_H__ */
