#ifndef __GENERAL_MIXER_IF_H__
#define __GENERAL_MIXER_IF_H__

#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include "general_mixer.h"

#ifdef __cplusplus
extern "C" {
#endif				/* __cplusplus */


#define GENERAL_MIXER_IF(obj)          GTK_CHECK_CAST (obj, general_mixer_if_get_type (), GeneralMixerIF)
#define GENERAL_MIXER_IF_CLASS(klass)  GTK_CHECK_CLASS_CAST (klass, general_mixer_if_get_type (), GeneralMixerIFClass)
#define IS_GENERAL_MIXER_IF(obj)       GTK_CHECK_TYPE (obj, general_mixer_if_get_type ())


  typedef struct _GeneralMixerIF GeneralMixerIF;
  typedef struct _GeneralMixerIFClass GeneralMixerIFClass;

  struct _GeneralMixerIF {
    GtkWindow window;

    GtkWidget *hbox;
    GtkWidget *vboxes[MAX_INPUTS];
    general_mixer *data;
  };

  struct _GeneralMixerIFClass {
    GtkWindowClass parent_class;

    void (*general_mixer_if) (GeneralMixerIF * a);
  };

  guint general_mixer_if_get_type(void);
  GtkWidget *general_mixer_if_new(general_mixer * xf);
  void general_mixer_if_close_cb(GtkWidget * widget, gpointer data);

#ifdef __cplusplus
}
#endif				/* __cplusplus */

#endif  /* __GENERAL_MIXER_IF_H__ */
