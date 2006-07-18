#ifndef __ATONAL_IF_H__
#define __ATONAL_IF_H__

#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include "atonal.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


#define ATONAL_IF(obj)          GTK_CHECK_CAST (obj, atonal_if_get_type (), AtonalIF)
#define ATONAL_IF_CLASS(klass)  GTK_CHECK_CLASS_CAST (klass, atonal_if_get_type (), AtonalIFClass)
#define IS_ATONAL_IF(obj)       GTK_CHECK_TYPE (obj, atonal_if_get_type ())


typedef struct _AtonalIF       AtonalIF;
typedef struct _AtonalIFClass  AtonalIFClass;

typedef struct {
  int index;
  GtkWidget *button;
} AtButton;
  
struct _AtonalIF
{
  GtkWindow window;
  
  AtButton buttons[AT_LENGTH];

  atonal *data;
};

struct _AtonalIFClass
{
  GtkWindowClass parent_class;

  void (* atonal_if) (AtonalIF *a);
};

GType         atonal_if_get_type        (void);
GtkWidget*     atonal_if_new             (atonal *rsq);
void atonal_if_close_cb(GtkWidget *widget, gpointer data);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __ATONAL_IF_H__ */
