#include <strings.h>
#include <gtk/gtk.h>

#include "none_config.h"
#include "aube.h"
#include "master.h"
#include "modulemenu.h"
#include "opsmenu.h"
#include "slider.h"
#include "inputoption.h"

extern int tick;

extern GtkWidget *master_daddy;

guint none_config_get_type(void);
static void none_config_class_init(NoneConfigClass * class);
static void none_config_init(NoneConfig * b);
void none_config_destroy(GtkWidget * widget, gpointer data);

/*
   this is a callback for module_class's 
 */
void
none_config_cb(module * u)
{
  GtkWidget *nc;

  nc = none_config_new(u);
  gtk_widget_show(nc);
}

guint
none_config_get_type()
{
  static guint b_type = 0;

  if (!b_type) {
    GtkTypeInfo b_info =
    {
      "NoneConfig",
      sizeof(NoneConfig),
      sizeof(NoneConfigClass),
      (GtkClassInitFunc) none_config_class_init,
      (GtkObjectInitFunc) none_config_init,
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

static gint none_config_signals[LAST_SIGNAL] =
{0};

static void
none_config_class_init(NoneConfigClass * class)
{
  GtkObjectClass *object_class;

  object_class = (GtkObjectClass *) class;

  gtk_object_class_add_signals(object_class, none_config_signals, LAST_SIGNAL);
  class->none_config = NULL;
}

static void
none_config_init(NoneConfig * none_config)
{
}

GtkWidget *
none_config_new(module * u)
{
  NoneConfig *none_config;
  GtkWidget *vbox;
  GtkWidget *frame;
  GtkWidget *widget;

  none_config = gtk_type_new(none_config_get_type());

  gtk_window_set_title(GTK_WINDOW(none_config), u->u_label);

  gtk_container_border_width(GTK_CONTAINER(none_config), 1);

#if 0
  /*
     when the window is given the "delete_event" signal - this is
     * given by the window manager - usually the close option or on the
     * titlebar - we ask it to call the delete_event() function
     * as defined above. The data passed to the callback function is
     * NULL and is ignored in the callback. 
   */
  gtk_signal_connect(GTK_OBJECT(none_config), "delete_event",
		     GTK_SIGNAL_FUNC(delete_event), NULL);

  /*
     here we connect the "destroy" event to a signal handler.
     * This event occurs when we call gtk_widget_destroy() on the
     * window, or if we return "TRUE" in the "delete_event" callback. 
   */
  gtk_signal_connect(GTK_OBJECT(none_config), "destroy",
		     GTK_SIGNAL_FUNC(destroy), NULL);
#endif

  frame = gtk_frame_new(u->u_label);
  gtk_container_add(GTK_CONTAINER(none_config), frame);
  gtk_widget_show(frame);

  vbox = gtk_vbox_new(FALSE, 1);
  gtk_container_add(GTK_CONTAINER(frame), vbox);
  gtk_widget_show(vbox);

  widget = gtk_label_new("This module has no options.");
  gtk_box_pack_start(GTK_BOX(vbox), widget, FALSE, FALSE, 2);
  gtk_widget_show(widget);

  widget = gtk_button_new_with_label("Ok");
  gtk_box_pack_start(GTK_BOX(vbox), widget, FALSE, FALSE, 2);
  gtk_signal_connect(GTK_OBJECT(widget), "clicked",
		     GTK_SIGNAL_FUNC(none_config_destroy), none_config);
  gtk_widget_show(widget);

  return GTK_WIDGET(none_config);
}

void
none_config_destroy(GtkWidget * widget, gpointer data)
{
  gtk_widget_destroy(GTK_WIDGET(data));
}
