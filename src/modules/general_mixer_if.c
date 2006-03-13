#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <gtk/gtk.h>

#include "general_mixer_if.h"
#include "aube.h"
#include "master.h"
#include "slider.h"
#include "inputoption.h"
#include "outputlabel.h"

guint general_mixer_if_get_type(void);
static void general_mixer_if_class_init(GeneralMixerIFClass * class);
static void general_mixer_if_init(GeneralMixerIF * b);
GtkWidget *general_mixer_if_new(general_mixer * xf);
void general_mixer_if_dismiss(GtkWidget * widget, gpointer data);
void general_mixer_if_hide_cb(GtkWidget * widget, gpointer data);
void general_mixer_if_close_cb(GtkWidget * widget, gpointer data);
void general_mixer_if_onoff_cb(GtkWidget * widget, gpointer data);
void general_mixer_if_add_input_cb(GtkWidget * widget, gpointer data);
void general_mixer_if_remove_input_cb(GtkWidget * widget, gpointer data);
void general_mixer_if_add_input(GeneralMixerIF * general_mixer_if, int i);
void general_mixer_if_remove_input(GeneralMixerIF * general_mixer_if, int i);
void general_mixer_if_mute_cb(GtkWidget * widget, gpointer data);

guint
general_mixer_if_get_type()
{
  static guint b_type = 0;

  if (!b_type) {
    GtkTypeInfo b_info =
    {
      "GeneralMixerIF",
      sizeof(GeneralMixerIF),
      sizeof(GeneralMixerIFClass),
      (GtkClassInitFunc) general_mixer_if_class_init,
      (GtkObjectInitFunc) general_mixer_if_init,
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

static gint general_mixer_if_signals[LAST_SIGNAL] =
{0};

static void
general_mixer_if_class_init(GeneralMixerIFClass * class)
{
  GtkObjectClass *object_class;

  object_class = (GtkObjectClass *) class;

  gtk_object_class_add_signals(object_class, general_mixer_if_signals, LAST_SIGNAL);
  class->general_mixer_if = NULL;
}

static void
general_mixer_if_init(GeneralMixerIF * general_mixer_if)
{
}

GtkWidget *
general_mixer_if_new(general_mixer * xf)
{
  GeneralMixerIF *general_mixer_if;
  GtkWidget *vbox, *vbox2, *hbox;
  GtkWidget *button, *slider;
  gint i;

  general_mixer_if = gtk_type_new(general_mixer_if_get_type());

  general_mixer_if->data = xf;

  gtk_window_set_title(GTK_WINDOW(general_mixer_if), ((module *) general_mixer_if->data)->u_label);
  gtk_window_set_policy(GTK_WINDOW(general_mixer_if), TRUE, TRUE, TRUE);

  gtk_container_border_width(GTK_CONTAINER(general_mixer_if), 1);

#if 0
  gtk_signal_connect(GTK_OBJECT(general_mixer_if), "delete_event",
		     GTK_SIGNAL_FUNC(delete_event), NULL);
#endif

#if 1
  gtk_signal_connect(GTK_OBJECT(general_mixer_if), "destroy",
		     GTK_SIGNAL_FUNC(general_mixer_if_close_cb), general_mixer_if);
#endif

  vbox2 = gtk_vbox_new(FALSE, 5);
  gtk_container_add(GTK_CONTAINER(general_mixer_if), vbox2);
  gtk_widget_show(vbox2);

  /*
     I N P U T 
   */

  hbox = gtk_hbox_new(FALSE, 0);
  gtk_box_pack_start(GTK_BOX(vbox2), hbox, FALSE, FALSE, 0);
  gtk_widget_show(hbox);

  button = gtk_toggle_button_new_with_label("On");
  gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 0);
  gtk_toggle_button_set_state(GTK_TOGGLE_BUTTON(button), general_mixer_if->data->module.on);
  gtk_signal_connect(GTK_OBJECT(button), "clicked",
	GTK_SIGNAL_FUNC(general_mixer_if_onoff_cb), general_mixer_if->data);
  gtk_widget_show(button);

  button = opsmenu_new((module *) general_mixer_if->data, GTK_WIDGET(general_mixer_if), general_mixer_if_hide_cb, general_mixer_if_close_cb);
  gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 1);
  gtk_widget_show(button);

  button = gtk_button_new_with_label("Add");
  gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 2);
  gtk_signal_connect(GTK_OBJECT(button), "clicked",
	  GTK_SIGNAL_FUNC(general_mixer_if_add_input_cb), general_mixer_if);
  gtk_widget_show(button);

  button = gtk_button_new_with_label("Remove");
  gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 2);
  gtk_signal_connect(GTK_OBJECT(button), "clicked",
       GTK_SIGNAL_FUNC(general_mixer_if_remove_input_cb), general_mixer_if);
  gtk_widget_show(button);

  button = outputlabel_new((module *) GENERAL_MIXER_IF(general_mixer_if)->data, 0);
  gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 2);
  gtk_widget_show(button);

  general_mixer_if->hbox = gtk_hbox_new(FALSE, 0);
  gtk_box_pack_start(GTK_BOX(vbox2), general_mixer_if->hbox, TRUE, TRUE, 0);
  gtk_widget_show(general_mixer_if->hbox);

  vbox = gtk_vbox_new(FALSE, 0);
  gtk_box_pack_start(GTK_BOX(general_mixer_if->hbox), vbox, TRUE, FALSE, 0);
  gtk_widget_show(vbox);

  slider = slider_new("Master", &(general_mixer_if->data->master_vol), 0, 64, 1);
  gtk_box_pack_start(GTK_BOX(vbox), slider, TRUE, TRUE, 0);
  gtk_widget_show(slider);

  button = gtk_toggle_button_new_with_label("Mute");
  gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 2);
  gtk_toggle_button_set_state(GTK_TOGGLE_BUTTON(button),
			      general_mixer_if->data->master_mute);
  gtk_signal_connect(GTK_OBJECT(button), "clicked",
		     GTK_SIGNAL_FUNC(general_mixer_if_mute_cb),
		     &(general_mixer_if->data->master_mute));
  gtk_widget_show(button);

  button = gtk_vseparator_new();
  gtk_box_pack_start(GTK_BOX(general_mixer_if->hbox), button, TRUE, FALSE, 2);
  gtk_widget_show(button);

  for (i = 0; i < general_mixer_if->data->nr_active_channels; i++) {
    general_mixer_if_add_input(general_mixer_if, i);
  }

  return GTK_WIDGET(general_mixer_if);
}

void 
general_mixer_if_hide_cb(GtkWidget * widget, gpointer data)
{
  module *u;
  GeneralMixerIF *general_mixer_if;

  general_mixer_if = GENERAL_MIXER_IF(data);
  u = (module *) general_mixer_if->data;
  aube_module_remove_if(u);

  gtk_widget_destroy(GTK_WIDGET(data));
}

void 
general_mixer_if_close_cb(GtkWidget * widget, gpointer data)
{
  module *u;
  GeneralMixerIF *general_mixer_if;

  general_mixer_if = GENERAL_MIXER_IF(data);
  u = (module *) general_mixer_if->data;
  aube_remove_module(u);

  free((GENERAL_MIXER_IF(data))->data);
  gtk_widget_destroy(GTK_WIDGET(data));
}

void 
general_mixer_if_dismiss(GtkWidget * widget, gpointer data)
{
  gtk_widget_destroy(GTK_WIDGET(data));
}

void 
general_mixer_if_onoff_cb(GtkWidget * widget, gpointer data)
{
  aube_module_toggle((module *) data);
}

void 
general_mixer_if_add_input_cb(GtkWidget * widget, gpointer data)
{
  GeneralMixerIF *general_mixer_if;
  int i;

  general_mixer_if = GENERAL_MIXER_IF(data);
  if ((i = general_mixer_add_input(general_mixer_if->data)) > 0) {
    general_mixer_if_add_input(general_mixer_if, i);
  }
}

void 
general_mixer_if_remove_input_cb(GtkWidget * widget, gpointer data)
{
  GeneralMixerIF *general_mixer_if;
  int i;

  general_mixer_if = GENERAL_MIXER_IF(data);
  if ((i = general_mixer_remove_input(general_mixer_if->data)) > 0) {
    general_mixer_if_remove_input(general_mixer_if, i);
  }
}

void 
general_mixer_if_mute_cb(GtkWidget * widget, gpointer data)
{
  *((int *) data) = 1 - *((int *) data);
}

void 
general_mixer_if_add_input(GeneralMixerIF * general_mixer_if, int i)
{
  char buf[8];
  GtkWidget *vbox, *button, *slider;

  vbox = gtk_vbox_new(FALSE, 0);
  gtk_box_pack_start(GTK_BOX(general_mixer_if->hbox), vbox, TRUE, TRUE, 0);
  gtk_widget_show(vbox);

  sprintf(buf, "%d:", i);
  button = inputoption_new(buf,
		    (module *) GENERAL_MIXER_IF(general_mixer_if)->data, i);
  reread_inputs_cb(GTK_WIDGET(general_mixer_if), button);
  gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, TRUE, 0);
  gtk_widget_show(button);

  sprintf(buf, "%d", i);
  slider = slider_new(buf, &(general_mixer_if->data->vol[i]), 0, 64, 1);
  gtk_box_pack_start(GTK_BOX(vbox), slider, TRUE, TRUE, 0);
  gtk_widget_show(slider);

  button = gtk_toggle_button_new_with_label("Mute");
  gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 2);
  gtk_toggle_button_set_state(GTK_TOGGLE_BUTTON(button),
			      general_mixer_if->data->mute[i]);
  gtk_signal_connect(GTK_OBJECT(button), "clicked",
		     GTK_SIGNAL_FUNC(general_mixer_if_mute_cb),
		     &(general_mixer_if->data->mute[i]));
  gtk_widget_show(button);

  general_mixer_if->vboxes[i] = vbox;
}

void 
general_mixer_if_remove_input(GeneralMixerIF * general_mixer_if, int i)
{
  if (general_mixer_if->vboxes[i] != NULL) {
    gtk_widget_destroy(general_mixer_if->vboxes[i]);
    general_mixer_if->vboxes[i] = NULL;
  }
}
