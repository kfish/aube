#ifndef __LINEOUT_IF_H__
#define __LINEOUT_IF_H__

#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include "line.h"

#ifdef __cplusplus
extern "C" {
#endif				/* __cplusplus */


#define LINEOUT_IF(obj)          GTK_CHECK_CAST (obj, lineout_if_get_type (), LINEOUTIF)
#define LINEOUT_IF_CLASS(klass)  GTK_CHECK_CLASS_CAST (klass, lineout_if_get_type (), LINEOUTIFClass)
#define IS_LINEOUT_IF(obj)       GTK_CHECK_TYPE (obj, lineout_if_get_type ())


  typedef struct _LINEOUTIF LINEOUTIF;
  typedef struct _LINEOUTIFClass LINEOUTIFClass;

  struct _LINEOUTIF {
    GtkWindow window;

    GtkWidget *inputs_om;
    GtkWidget *inputsmenu;
    oss_out *data;
  };

  struct _LINEOUTIFClass {
    GtkWindowClass parent_class;

    void (*lineout_if) (LINEOUTIF * a);
  };

  GType lineout_if_get_type(void);
  GtkWidget *lineout_if_new(oss_out * oss_p);
  void lineout_if_close_cb(GtkWidget * widget, gpointer data);

#ifdef __cplusplus
}
#endif				/* __cplusplus */

#endif  /* __LINEOUT_IF_H__ */
