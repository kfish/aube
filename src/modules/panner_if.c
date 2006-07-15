#include <stdlib.h>
#include <strings.h>
#include <gtk/gtk.h>

#include "panner_if.h"
#include "aube.h"
#include "master.h"
#include "slider.h"
#include "inputoption.h"
#include "outputlabel.h"

extern int tick;

guint panner_if_get_type(void);
static void panner_if_class_init(PannerIFClass * class);
static void panner_if_init(PannerIF * b);
GtkWidget *panner_if_new(panner * pn);
void panner_if_dismiss(GtkWidget * widget, gpointer data);
void panner_if_hide_cb(GtkWidget * widget, gpointer data);
void panner_if_close_cb(GtkWidget * widget, gpointer data);
void panner_if_onoff_cb(GtkWidget * widget, gpointer data);

guint
panner_if_get_type()
{
  static guint b_type = 0;

  if (!b_type) {
    GtkTypeInfo b_info =
    {
      "PannerIF",
      sizeof(PannerIF),
      sizeof(PannerIFClass),
      (GtkClassInitFunc) panner_if_class_init,
      (GtkObjectInitFunc) panner_if_init,
      (GtkArgSetFunc) NULL,
      (GtkArgGetFunc) NULL,
    };

    b_type = gtk_type_unique(gtk_window_get_type(), &b_info);
  }
  return b_type;
}

enum {
  LAST_SIGNAL
};

static guint panner_if_signals[LAST_SIGNAL+1] =
{0};

static void
panner_if_class_init(PannerIFClass * class)
{
  GtkObjectClass *object_class;

  object_class = (GtkObjectClass *) class;

  gtk_object_class_add_signals(object_class, panner_if_signals, LAST_SIGNAL);
  class->panner_if = NULL;
}

static void
panner_if_init(PannerIF * panner_if)
{
}

GtkWidget *
panner_if_new(panner * pn)
{
  PannerIF *panner_if;
  GtkWidget *vbox, *vbox2, *hbox, *hbox2;
  GtkWidget *frame;
  GtkWidget *button;
  GtkWidget *slider;

  panner_if = gtk_type_new(panner_if_get_type());

  panner_if->data = pn;

  gtk_window_set_title(GTK_WINDOW(panner_if), ((module *) panner_if->data)->u_label);

  gtk_container_border_width(GTK_CONTAINER(panner_if), 1);

#if 0
  /*
     when the window is given the "delete_event" signal - this is
     * given by the window manager - usually the close option or on the
     * titlebar - we ask it to call the delete_event() function
     * as defined above. The data passed to the callback function is
     * NULL and is ignored in the callback. 
   */
  gtk_signal_connect(GTK_OBJECT(panner_if), "delete_event",
		     GTK_SIGNAL_FUNC(delete_event), NULL);
#endif

#if 1
  /*
     here we connect the "destroy" event to a signal handler.
     * This event occurs when we call gtk_widget_destroy() on the
     * window, or if we return "TRUE" in the "delete_event" callback. 
   */
  gtk_signal_connect(GTK_OBJECT(panner_if), "destroy",
		     GTK_SIGNAL_FUNC(panner_if_close_cb), panner_if);
#endif

  vbox2 = gtk_vbox_new(FALSE, 5);
  gtk_container_add(GTK_CONTAINER(panner_if), vbox2);
  gtk_widget_show(vbox2);

  /*
     I N P U T 
   */

  hbox = gtk_hbox_new(FALSE, 0);
  gtk_box_pack_start(GTK_BOX(vbox2), hbox, FALSE, TRUE, 0);
  gtk_widget_show(hbox);

  button = gtk_toggle_button_new_with_label("On");
  gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 1);
  gtk_toggle_button_set_state(GTK_TOGGLE_BUTTON(button), panner_if->data->module.on);
  gtk_signal_connect(GTK_OBJECT(button), "clicked",
		     GTK_SIGNAL_FUNC(panner_if_onoff_cb), panner_if->data);
  gtk_widget_show(button);

  button = opsmenu_new((module *) panner_if->data, GTK_WIDGET(panner_if),
		       panner_if_hide_cb, panner_if_close_cb);
  gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 4);
  gtk_widget_show(button);

  button = outputlabel_new((module *) PANNER_IF(panner_if)->data, 0);
  gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 2);
  gtk_widget_show(button);

  hbox = gtk_hbox_new(FALSE, 0);
  gtk_box_pack_start(GTK_BOX(vbox2), hbox, FALSE, FALSE, 0);
  gtk_widget_show(hbox);

  button = inputoption_new((char *) "In:", (module *) PANNER_IF(panner_if)->data, 0);
  gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, TRUE, 2);
  gtk_widget_show(button);

  hbox2 = gtk_hbox_new(FALSE, 5);
  gtk_box_pack_start(GTK_BOX(vbox2), hbox2, TRUE, TRUE, 0);
  gtk_widget_show(hbox2);

  frame = gtk_frame_new(NULL);
  gtk_box_pack_start(GTK_BOX(hbox2), frame, TRUE, TRUE, 0);
/*
   gtk_container_border_width(GTK_CONTAINER(frame), 4);
 */
  gtk_widget_show(frame);

  vbox = gtk_vbox_new(FALSE, 5);
  gtk_container_add (GTK_CONTAINER(frame), vbox);
  gtk_widget_show(vbox);

  slider = slider_new("Input", &(panner_if->data->vol), 0, 64, 1);
  gtk_box_pack_start(GTK_BOX(vbox), slider, TRUE, TRUE, 0);
  gtk_widget_show(slider);

  slider = slider_new("Pan", &(panner_if->data->pan), 0, 32, 0);
  gtk_box_pack_start(GTK_BOX(vbox), slider, FALSE, TRUE, 0);
  gtk_widget_show(slider);

  return GTK_WIDGET(panner_if);
}

void 
panner_if_hide_cb(GtkWidget * widget, gpointer data)
{
  module *u;
  PannerIF *panner_if;

  panner_if = PANNER_IF(data);
  u = (module *) panner_if->data;
  aube_module_remove_if(u);

  gtk_widget_destroy(GTK_WIDGET(data));
}

void 
panner_if_close_cb(GtkWidget * widget, gpointer data)
{
  module *u;
  PannerIF *panner_if;

  panner_if = PANNER_IF(data);
  u = (module *) panner_if->data;
  aube_remove_module(u);

  free((PANNER_IF(data))->data);
  gtk_widget_destroy(GTK_WIDGET(data));
}

void 
panner_if_dismiss(GtkWidget * widget, gpointer data)
{
  gtk_widget_destroy(GTK_WIDGET(data));
}

void 
panner_if_onoff_cb(GtkWidget * widget, gpointer data)
{
  aube_module_toggle((module *) data);
}
