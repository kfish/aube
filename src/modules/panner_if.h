#ifndef __PANNER_IF_H__
#define __PANNER_IF_H__

#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include "panner.h"

#ifdef __cplusplus
extern "C" {
#endif				/* __cplusplus */


#define PANNER_IF(obj)          GTK_CHECK_CAST (obj, panner_if_get_type (), PannerIF)
#define PANNER_IF_CLASS(klass)  GTK_CHECK_CLASS_CAST (klass, panner_if_get_type (), PannerIFClass)
#define IS_PANNER_IF(obj)       GTK_CHECK_TYPE (obj, panner_if_get_type ())


  typedef struct _PannerIF PannerIF;
  typedef struct _PannerIFClass PannerIFClass;

  struct _PannerIF {
    GtkWindow window;

    panner *data;
  };

  struct _PannerIFClass {
    GtkWindowClass parent_class;

    void (*panner_if) (PannerIF * a);
  };

  guint panner_if_get_type(void);
  GtkWidget *panner_if_new(panner * pn);
  void panner_if_close_cb(GtkWidget * widget, gpointer data);

#ifdef __cplusplus
}
#endif				/* __cplusplus */

#endif				/* __PANNER_IF_H__ */
