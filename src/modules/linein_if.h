
#ifndef __LINEIN_IF_H__
#define __LINEIN_IF_H__

#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include "line.h"

#ifdef __cplusplus
extern "C" {
#endif				/* __cplusplus */


#define LINEIN_IF(obj)          GTK_CHECK_CAST (obj, linein_if_get_type (), LINEINIF)
#define LINEIN_IF_CLASS(klass)  GTK_CHECK_CLASS_CAST (klass, linein_if_get_type (), LINEINIFClass)
#define IS_LINEIN_IF(obj)       GTK_CHECK_TYPE (obj, linein_if_get_type ())


  typedef struct _LINEINIF LINEINIF;
  typedef struct _LINEINIFClass LINEINIFClass;

  struct _LINEINIF {
    GtkWindow window;

    GtkWidget *inputs_om;
    GtkWidget *inputsmenu;
    oss_in *data;
  };

  struct _LINEINIFClass {
    GtkWindowClass parent_class;

    void (*linein_if) (LINEINIF * a);
  };

  GType linein_if_get_type(void);
  GtkWidget *linein_if_new(oss_in * oss_p);
  void linein_if_close_cb(GtkWidget * widget, gpointer data);

#ifdef __cplusplus
}

#endif				/* __cplusplus */
#endif				/* __LINEIN_IF_H__ */
