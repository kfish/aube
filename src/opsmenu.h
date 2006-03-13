
#ifndef __OPSMENU_H__
#define __OPSMENU_H__

#include <gdk/gdk.h>
#include <gtk/gtkvbox.h>
#include "aube.h"

#ifdef __cplusplus
extern "C" {
#endif				/*
				   __cplusplus 
				 */


#define OPSMENU(obj)          GTK_CHECK_CAST (obj, opsmenu_get_type (), Opsmenu)
#define OPSMENU_CLASS(klass)  GTK_CHECK_CLASS_CAST (klass, opsmenu_get_type (), OpsmenuClass)
#define IS_OPSMENU(obj)       GTK_CHECK_TYPE (obj, opsmenu_get_type ())


  typedef struct _Opsmenu Opsmenu;
  typedef struct _OpsmenuClass OpsmenuClass;

  struct _Opsmenu {
    GtkMenuBar menubar;

    module *module;
  };

  struct _OpsmenuClass {
    GtkMenuBarClass parent_class;

    void (*opsmenu) (Opsmenu * a);
  };

  guint opsmenu_get_type(void);
  GtkWidget *opsmenu_new(module * module, GtkWidget * parent_if, void *if_hide_cb, void *if_close_cb);


#ifdef __cplusplus
}

#endif				/*
				   __cplusplus 
				 */
#endif				/*
				   __OPSMENU_H__ 
				 */
