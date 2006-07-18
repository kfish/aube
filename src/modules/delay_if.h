#ifndef __DELAY_IF_H__
#define __DELAY_IF_H__

#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include "delay.h"

#ifdef __cplusplus
extern "C" {
#endif				/* __cplusplus */


#define DELAY_IF(obj)          GTK_CHECK_CAST (obj, delay_if_get_type (), DelayIF)
#define DELAY_IF_CLASS(klass)  GTK_CHECK_CLASS_CAST (klass, delay_if_get_type (), DelayIFClass)
#define IS_DELAY_IF(obj)       GTK_CHECK_TYPE (obj, delay_if_get_type ())


  typedef struct _DelayIF DelayIF;
  typedef struct _DelayIFClass DelayIFClass;

  struct _DelayIF {
    GtkWindow window;

    GtkWidget *replace_menuitem;
    delay *data;
  };

  struct _DelayIFClass {
    GtkWindowClass parent_class;

    void (*delay_if) (DelayIF * a);
  };

  GType delay_if_get_type(void);
  GtkWidget *delay_if_new(delay * dl);
  void delay_if_close_cb(GtkWidget * widget, gpointer data);

#ifdef __cplusplus
}
#endif				/* __cplusplus */

#endif				/* __DELAY_IF_H__ */
