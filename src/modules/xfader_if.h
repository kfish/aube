#ifndef __XFADER_IF_H__
#define __XFADER_IF_H__

#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include "xfader.h"

#ifdef __cplusplus
extern "C" {
#endif				/* __cplusplus */


#define XFADER_IF(obj)          GTK_CHECK_CAST (obj, xfader_if_get_type (), XFaderIF)
#define XFADER_IF_CLASS(klass)  GTK_CHECK_CLASS_CAST (klass, xfader_if_get_type (), XFaderIFClass)
#define IS_XFADER_IF(obj)       GTK_CHECK_TYPE (obj, xfader_if_get_type ())


  typedef struct _XFaderIF XFaderIF;
  typedef struct _XFaderIFClass XFaderIFClass;

  struct _XFaderIF {
    GtkWindow window;

    xfader *data;
  };

  struct _XFaderIFClass {
    GtkWindowClass parent_class;

    void (*xfader_if) (XFaderIF * a);
  };

  guint xfader_if_get_type(void);
  GtkWidget *xfader_if_new(xfader * xf);
  void xfader_if_close_cb(GtkWidget * widget, gpointer data);

#ifdef __cplusplus
}
#endif				/* __cplusplus */
#endif  /* __XFADER_IF_H__ */
