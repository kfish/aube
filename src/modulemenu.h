
#ifndef __UNITMENU_H__
#define __UNITMENU_H__

#include <gdk/gdk.h>
#include <gtk/gtkvbox.h>
#include "aube.h"

#ifdef __cplusplus
extern "C" {
#endif				/*
				   __cplusplus 
				 */


#define UNITMENU(obj)          GTK_CHECK_CAST (obj, modulemenu_get_type (), Unitmenu)
#define UNITMENU_CLASS(klass)  GTK_CHECK_CLASS_CAST (klass, modulemenu_get_type (), UnitmenuClass)
#define IS_UNITMENU(obj)       GTK_CHECK_TYPE (obj, modulemenu_get_type ())


  typedef struct _Unitmenu Unitmenu;
  typedef struct _UnitmenuClass UnitmenuClass;

  typedef struct {
    module *this_module;
    module *selected_module;
  } modulepack;

  struct _Unitmenu {
    GtkMenu menu;

    int (*include_criterion) (module * u1, module * u2);
    void (*select_action) (GtkWidget * widget, gpointer data);
    modulepack *up;
  };

  struct _UnitmenuClass {
    GtkMenuClass parent_class;

    void (*modulemenu) (Unitmenu * a);
  };

  guint modulemenu_get_type(void);
  GtkWidget *modulemenu_new(module * this_module, int *include_criterion(), void *select_action());


#ifdef __cplusplus
}

#endif				/*
				   __cplusplus 
				 */
#endif				/*
				   __UNITMENU_H__ 
				 */
