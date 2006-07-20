#include <stdlib.h>
#include <strings.h>
#include <gtk/gtk.h>

#include "delay_if.h"
#include "aube.h"
#include "master.h"
#include "modulemenu.h"
#include "opsmenu.h"
#include "slider.h"
#include "inputoption.h"
#include "outputlabel.h"

extern int tick;

extern GtkWidget *master_daddy;

static void delay_if_class_init(DelayIFClass * klass);
static void delay_if_init(DelayIF * b);
GtkWidget *delay_if_new(delay * dl);
void delay_if_dismiss(GtkWidget * widget, gpointer data);
void delay_if_hide_cb(GtkWidget * widget, gpointer data);
void delay_if_close_cb(GtkWidget * widget, gpointer data);
void delay_if_onoff_cb(GtkWidget * widget, gpointer data);
void delay_if_change_replace_menu_cb(GtkWidget * widget, gpointer data);

GType
delay_if_get_type(void)
{
  static GType b_type = 0;

  if (!b_type) {
    static const GTypeInfo b_info =
    {
      sizeof(DelayIFClass),
      NULL, /* base_init */
	  NULL, /* base_finalise */
      (GClassInitFunc) delay_if_class_init,
	  NULL, /* class_finalize */
	  NULL, /* class_data */
      sizeof(DelayIF),
	  0, /* n_preallocs */
	  (GInstanceInitFunc)delay_if_init,
    };

    b_type = g_type_register_static(GTK_TYPE_WINDOW,
                                                      "DelayIF",
	                                                   &b_info, 0);
  }
  return b_type;
}

static void
delay_if_class_init(DelayIFClass * klass)
{
 
}

static void
delay_if_init(DelayIF * delay_if)
{
}

GtkWidget *
delay_if_new(delay * dl)
{
  DelayIF *delay_if;
  GtkWidget *vbox, *vbox2, *hbox, *hbox2, *hbox3;
  GtkWidget *frame;
  GtkWidget *widget;
  GtkWidget *slider;

  delay_if = gtk_type_new(delay_if_get_type());

  delay_if->data = dl;

  gtk_window_set_title(GTK_WINDOW(delay_if), ((module *) delay_if->data)->u_label);

  gtk_container_border_width(GTK_CONTAINER(delay_if), 1);

#if 0
  /*
     when the window is given the "delete_event" signal - this is
     * given by the window manager - usually the close option or on the
     * titlebar - we ask it to call the delete_event() function
     * as defined above. The data passed to the callback function is
     * NULL and is ignored in the callback. 
   */
  gtk_signal_connect(GTK_OBJECT(delay_if), "delete_event",
		     GTK_SIGNAL_FUNC(delete_event), NULL);
#endif

#if 1
  /*
     here we connect the "destroy" event to a signal handler.
     * This event occurs when we call gtk_widget_destroy() on the
     * window, or if we return "TRUE" in the "delete_event" callback. 
   */
  gtk_signal_connect(GTK_OBJECT(delay_if), "destroy",
		     GTK_SIGNAL_FUNC(delay_if_close_cb), delay_if);
#endif

  vbox2 = gtk_vbox_new(FALSE, 5);
  gtk_container_add(GTK_CONTAINER(delay_if), vbox2);
  gtk_widget_show(vbox2);

  hbox = gtk_hbox_new(FALSE, 1);
  gtk_box_pack_start(GTK_BOX(vbox2), hbox, FALSE, FALSE, 1);
  gtk_widget_show(hbox);

  widget = gtk_toggle_button_new_with_label("On");
  gtk_box_pack_start(GTK_BOX(hbox), widget, FALSE, FALSE, 1);
  gtk_toggle_button_set_state(GTK_TOGGLE_BUTTON(widget), delay_if->data->module.on);
  gtk_signal_connect(GTK_OBJECT(widget), "clicked",
		     GTK_SIGNAL_FUNC(delay_if_onoff_cb), delay_if->data);
  gtk_widget_show(widget);


  widget = opsmenu_new((module *) delay_if->data, GTK_WIDGET(delay_if),
		       delay_if_hide_cb, delay_if_close_cb);
  gtk_box_pack_start(GTK_BOX(hbox), widget, FALSE, FALSE, 2);
  gtk_widget_show(widget);

  widget = outputlabel_new((module *) DELAY_IF(delay_if)->data, 0);
  gtk_box_pack_start(GTK_BOX(hbox), widget, TRUE, TRUE, 2);
  gtk_widget_show(widget);


  /*
     I N P U T 
   */

  widget = inputoption_new((char *) "In:", (module *) DELAY_IF(delay_if)->data, 0);
  gtk_box_pack_start(GTK_BOX(vbox2), widget, FALSE, TRUE, 2);
  gtk_widget_show(widget);

  hbox2 = gtk_hbox_new(FALSE, 5);
  gtk_box_pack_start(GTK_BOX(vbox2), hbox2, TRUE, TRUE, 0);
  gtk_widget_show(hbox2);

  frame = gtk_frame_new(NULL);
  gtk_box_pack_start(GTK_BOX(hbox2), frame, TRUE, TRUE, 1);
/*
   gtk_container_border_width(GTK_CONTAINER(frame), 4);
 */
  gtk_widget_show(frame);

  vbox = gtk_vbox_new(FALSE, 5);
  gtk_container_add(GTK_CONTAINER(frame), vbox);
  gtk_widget_show(vbox);

  hbox3 = gtk_hbox_new(FALSE, 5);
  gtk_box_pack_start(GTK_BOX(vbox), hbox3, TRUE, TRUE, 0);
  gtk_widget_show(hbox3);

#if 0
  slider = slider_int_new("Input Vol", &(delay_if->data->input_vol), 0, 64, 1);
  gtk_box_pack_start(GTK_BOX(hbox3), slider, TRUE, FALSE, 0);
  gtk_widget_show(slider);
#endif

  slider = slider_int_new("Delay", &(delay_if->data->offset), 0, DELAY_BUFFER_LENGTH / 2, 1);
  gtk_box_pack_start(GTK_BOX(hbox3), slider, TRUE, FALSE, 0);
  gtk_widget_show(slider);

  slider = slider_int_new("Delay Vol", &(delay_if->data->delay_vol), 0, 64, 1);
  gtk_box_pack_start(GTK_BOX(hbox3), slider, TRUE, FALSE, 0);
  gtk_widget_show(slider);

  slider = slider_int_new("Delay Pan", &(delay_if->data->pan), 0, 32, 0);
  gtk_box_pack_start(GTK_BOX(vbox), slider, FALSE, TRUE, 0);
  gtk_widget_show(slider);

  return GTK_WIDGET(delay_if);
}

void 
delay_if_hide_cb(GtkWidget * widget, gpointer data)
{
  module *u;
  DelayIF *delay_if;

  delay_if = DELAY_IF(data);
  u = (module *) delay_if->data;
  aube_module_remove_if(u);

  gtk_widget_destroy(GTK_WIDGET(data));
}

void 
delay_if_close_cb(GtkWidget * widget, gpointer data)
{
  module *u;
  DelayIF *delay_if;

  delay_if = DELAY_IF(data);
  u = (module *) delay_if->data;
  aube_remove_module(u);

  free((DELAY_IF(data))->data);
  gtk_widget_destroy(GTK_WIDGET(data));
}

void 
delay_if_dismiss(GtkWidget * widget, gpointer data)
{
  gtk_widget_destroy(GTK_WIDGET(data));
}

void 
delay_if_onoff_cb(GtkWidget * widget, gpointer data)
{
  aube_module_toggle((module *) data);
}

void 
delay_if_change_replace_menu_cb(GtkWidget * widget, gpointer data)
{
  GtkWidget *menu;

  menu = modulemenu_new((module *) DELAY_IF(data)->data, (void *) aube_module_cmp_type, (void *) module_replace_cb);
  gtk_menu_item_remove_submenu(GTK_MENU_ITEM(DELAY_IF(data)->replace_menuitem));
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(DELAY_IF(data)->replace_menuitem), menu);
}
