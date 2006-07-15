#include <stdlib.h>
#include <strings.h>
#include <gtk/gtk.h>

#include "panner4_if.h"
#include "aube.h"
#include "master.h"
#include "slider.h"
#include "inputoption.h"
#include "outputlabel.h"

extern int tick;

guint panner4_if_get_type(void);
static void panner4_if_class_init(Panner4IFClass * class);
static void panner4_if_init(Panner4IF * b);
GtkWidget *panner4_if_new(panner4 * pn);
void panner4_if_dismiss(GtkWidget * widget, gpointer data);
void panner4_if_hide_cb(GtkWidget * widget, gpointer data);
void panner4_if_close_cb(GtkWidget * widget, gpointer data);
void panner4_if_onoff_cb(GtkWidget * widget, gpointer data);

guint
panner4_if_get_type()
{
  static guint b_type = 0;

  if (!b_type) {
    GtkTypeInfo b_info =
    {
      "Panner4IF",
      sizeof(Panner4IF),
      sizeof(Panner4IFClass),
      (GtkClassInitFunc) panner4_if_class_init,
      (GtkObjectInitFunc) panner4_if_init,
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

static guint panner4_if_signals[LAST_SIGNAL+1] =
{0};

static void
panner4_if_class_init(Panner4IFClass * class)
{
  GtkObjectClass *object_class;

  object_class = (GtkObjectClass *) class;

  gtk_object_class_add_signals(object_class, panner4_if_signals, LAST_SIGNAL);
  class->panner4_if = NULL;
}

static void
panner4_if_init(Panner4IF * panner4_if)
{
}

GtkWidget *
panner4_if_new(panner4 * pn)
{
  Panner4IF *panner4_if;
  GtkWidget *vbox, *vbox2, *hbox, *hbox2, *hbox3;
  GtkWidget *frame;
  GtkWidget *button;
  GtkWidget *slider;

  panner4_if = gtk_type_new(panner4_if_get_type());

  panner4_if->data = pn;

  gtk_window_set_title(GTK_WINDOW(panner4_if), ((module *) panner4_if->data)->u_label);

  gtk_container_border_width(GTK_CONTAINER(panner4_if), 1);

#if 0
  /*
     when the window is given the "delete_event" signal - this is
     * given by the window manager - usually the close option or on the
     * titlebar - we ask it to call the delete_event() function
     * as defined above. The data passed to the callback function is
     * NULL and is ignored in the callback. 
   */
  gtk_signal_connect(GTK_OBJECT(panner4_if), "delete_event",
		     GTK_SIGNAL_FUNC(delete_event), NULL);
#endif

#if 1
  /*
     here we connect the "destroy" event to a signal handler.
     * This event occurs when we call gtk_widget_destroy() on the
     * window, or if we return "TRUE" in the "delete_event" callback. 
   */
  gtk_signal_connect(GTK_OBJECT(panner4_if), "destroy",
		     GTK_SIGNAL_FUNC(panner4_if_close_cb), panner4_if);
#endif

  vbox2 = gtk_vbox_new(FALSE, 5);
  gtk_container_add(GTK_CONTAINER(panner4_if), vbox2);
  gtk_widget_show(vbox2);

  /*
     I N P U T 
   */

  hbox = gtk_hbox_new(FALSE, 0);
  gtk_box_pack_start (GTK_BOX (vbox2), hbox, FALSE, FALSE, 1);
  gtk_widget_show(hbox);

  button = gtk_toggle_button_new_with_label("On");
  gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 1);
  gtk_toggle_button_set_state(GTK_TOGGLE_BUTTON(button), panner4_if->data->module.on);
  gtk_signal_connect(GTK_OBJECT(button), "clicked",
		     GTK_SIGNAL_FUNC(panner4_if_onoff_cb), panner4_if->data);
  gtk_widget_show(button);

  button = opsmenu_new((module *) panner4_if->data, GTK_WIDGET(panner4_if),
		       panner4_if_hide_cb, panner4_if_close_cb);
  gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 4);
  gtk_widget_show(button);

  hbox = gtk_hbox_new(FALSE, 0);
  gtk_box_pack_start (GTK_BOX (vbox2), hbox, FALSE, FALSE, 1);
  gtk_widget_show(hbox);

  button = inputoption_new((char *) "In:", (module *) PANNER4_IF(panner4_if)->data, 0);
  gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, TRUE, 2);
  gtk_widget_show(button);

  hbox = gtk_hbox_new(FALSE, 5);
  gtk_box_pack_start(GTK_BOX(vbox2), hbox, FALSE, TRUE, 0);
  gtk_widget_show(hbox);

  button = outputlabel_new((module *) PANNER4_IF(panner4_if)->data, 0);
  gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, TRUE, 2);
  gtk_widget_show(button);

  button = outputlabel_new((module *) PANNER4_IF(panner4_if)->data, 1);
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

  hbox = gtk_hbox_new(FALSE, 0);
  gtk_container_add(GTK_CONTAINER(frame), hbox);
  gtk_widget_show(hbox);

  vbox = gtk_vbox_new(FALSE, 5);
  gtk_box_pack_start(GTK_BOX(hbox), vbox, TRUE, TRUE, 0);
  gtk_widget_show(vbox);

  hbox3 = gtk_hbox_new(FALSE, 5);
  gtk_box_pack_start(GTK_BOX(vbox), hbox3, TRUE, TRUE, 0);
  gtk_widget_show(hbox3);

  slider = slider_new("Fade", &(panner4_if->data->fade), 0, 32, 1);
  gtk_box_pack_start(GTK_BOX(hbox3), slider, TRUE, FALSE, 0);
  gtk_widget_show(slider);

  slider = slider_new("Pan", &(panner4_if->data->pan), 0, 32, 0);
  gtk_box_pack_start(GTK_BOX(vbox), slider, FALSE, TRUE, 0);
  gtk_widget_show(slider);


  return GTK_WIDGET(panner4_if);
}

void 
panner4_if_hide_cb(GtkWidget * widget, gpointer data)
{
  module *u;
  Panner4IF *panner4_if;

  panner4_if = PANNER4_IF(data);
  u = (module *) panner4_if->data;
  aube_module_remove_if(u);

  gtk_widget_destroy(GTK_WIDGET(data));
}

void 
panner4_if_close_cb(GtkWidget * widget, gpointer data)
{
  module *u;
  Panner4IF *panner4_if;

  panner4_if = PANNER4_IF(data);
  u = (module *) panner4_if->data;
  aube_remove_module(u);

  free((PANNER4_IF(data))->data);
  gtk_widget_destroy(GTK_WIDGET(data));
}

void 
panner4_if_dismiss(GtkWidget * widget, gpointer data)
{
  gtk_widget_destroy(GTK_WIDGET(data));
}

void 
panner4_if_onoff_cb(GtkWidget * widget, gpointer data)
{
  aube_module_toggle((module *) data);
}
