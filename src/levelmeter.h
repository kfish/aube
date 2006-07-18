
#ifndef __LEVELMETER_H__
#define __LEVELMETER_H__

#include <gdk/gdk.h>
#include <gtk/gtkwidget.h>

#ifdef __cplusplus
extern "C" {
#endif				/*
				   __cplusplus 
				 */

#define LEVELMETER(obj)		GTK_CHECK_CAST(obj, levelmeter_get_type (), LevelMeter)
#define LEVELMETER_CLASS(klass)	GTK_CHECK_CLASS_CAST(klass, levelmeter_get_type (), LevelMeterClass)
#define IS_LEVELMETER(obj)		GTK_CHECK_TYPE(obj, levelmeter_get_type())

  typedef struct _LevelMeter LevelMeter;
  typedef struct _LevelMeterClass LevelMeterClass;

  struct _LevelMeter {
    GtkWidget widget;

    guint level;
  };

  struct _LevelMeterClass {
    GtkWidgetClass parent_class;
  };

  GtkWidget *levelmeter_new(guint state);
  GType levelmeter_get_type(void);
  guint levelmeter_get_level(LevelMeter * levelmeter);
  void levelmeter_set_level(LevelMeter * levelmeter, guint level);

#ifdef __cpluscplus
}

#endif
#endif				/*
				   __LEVELMETER_H__ 
				 */
