
#ifndef __MASTER_H__
#define __MASTER_H__

#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include "aube.h"
#include "opsmenu.h"

#ifdef __cplusplus
extern "C" {
#endif				/*
				   __cplusplus 
				 */


#define MASTER(obj)          GTK_CHECK_CAST (obj, master_get_type (), Master)
#define MASTER_CLASS(klass)  GTK_CHECK_CLASS_CAST (klass, master_get_type (), MasterClass)
#define IS_MASTER(obj)       GTK_CHECK_TYPE (obj, master_get_type ())


  typedef struct _Master Master;
  typedef struct _MasterClass MasterClass;

  struct _Master {
    GtkWindow window;

    aube_data *data;
  };

  struct _MasterClass {
    GtkWindowClass parent_class;

    void (*master) (Master * a);
  };

  GType master_get_type(void);
  GtkWidget *master_new(void);
void master_ack_channels_modified(void);
  void module_config_cb(GtkWidget * widget, gpointer data);
  void module_clone_cb(GtkWidget * widget, gpointer data);
  void module_replace_cb(GtkWidget * widget, gpointer data);
  void change_replace_menu_cb(GtkWidget * widget, gpointer data);

#ifdef __cplusplus
}

#endif				/*
				   __cplusplus 
				 */
#endif				/*
				   __MASTER_H__ 
				 */
