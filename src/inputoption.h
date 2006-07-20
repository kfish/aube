

#ifndef __INPUTOPTION_H__
#define __INPUTOPTION_H__

#include <gdk/gdk.h>
#include <gtk/gtkframe.h>
#include "aube.h"

#ifdef __cplusplus
extern "C" {
#endif				/*
				   __cplusplus 
				 */


#define INPUTOPTION(obj)          GTK_CHECK_CAST (obj, inputoption_get_type (), Inputoption)
#define INPUTOPTION_CLASS(klass)  GTK_CHECK_CLASS_CAST (klass, inputoption_get_type (), InputoptionClass)
#define IS_INPUTOPTION(obj)       GTK_CHECK_TYPE (obj, inputoption_get_type ())


  typedef struct _Inputoption Inputoption;
  typedef struct _InputoptionClass InputoptionClass;

  typedef struct {
    channel *ch;
    gpointer data;
  } inputpack;

  struct _Inputoption {
    GtkFrame frame;

    GtkWidget *optionmenu;
    GtkWidget *inputsmenu;
    module *module;
    int input_i;
    inputpack *ip;
    guint handler_id;
  };

  struct _InputoptionClass {
    GtkWindowClass parent_class;

    void (*inputoption) (Inputoption * a);
  };

  GType inputoption_get_type(void);
  GtkWidget *inputoption_new(char *label_str, module * module, int input_i);
  void reread_inputs_cb(GtkWidget * widget, gpointer data);


#ifdef __cplusplus
}

#endif				/*
				   __cplusplus 
				 */
#endif				/*
				   __INPUTOPTION_H__ 
				 */
