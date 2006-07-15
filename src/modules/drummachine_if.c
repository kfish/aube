#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <gtk/gtk.h>
#include "aube.h"
#include "master.h"
#include "opsmenu.h"
#include "outputlabel.h"
#include "drummachine_if.h"
#include "slider.h"


extern drummachine *drummachine_new(void);

extern int tick;
extern char *note_names[97];

guint drummachine_if_get_type(void);
static void drummachine_if_class_init(DrumMachineIFClass * class);
static void drummachine_if_init(DrumMachineIF * b);
GtkWidget *drummachine_if_new(drummachine * rsq);
void drummachine_if_dismiss(GtkWidget * widget, gpointer data);

void drummachine_if_update_dm(GtkWidget * widget, gpointer data);
void drummachine_if_hide_cb(GtkWidget * widget, gpointer data);
void drummachine_if_close_cb(GtkWidget * widget, gpointer data);
void drummachine_if_onoff_cb(GtkWidget * widget, gpointer data);
void drummachine_if_set_note_cb(GtkWidget * widget, gpointer note);
void dm_restart_cb(GtkWidget * widget, gpointer data);
void dm_clear_cb(GtkWidget * widget, gpointer data);
void dm_chaos_cb(GtkWidget * widget, gpointer data);
void dm_play_once_cb(GtkWidget * widget, gpointer data);
void dm_play_44_cb(GtkWidget * widget, gpointer data);
void dm_transpose_u12_cb(GtkWidget * widget, gpointer data);
void dm_transpose_d12_cb(GtkWidget * widget, gpointer data);
void dm_transpose_u1_cb(GtkWidget * widget, gpointer data);
void dm_transpose_d1_cb(GtkWidget * widget, gpointer data);
void dm_lshift_cb(GtkWidget * widget, gpointer data);
void dm_rshift_cb(GtkWidget * widget, gpointer data);

void dm_shift_left_cb (GtkWidget * widget, gpointer data);
void dm_shift_right_cb (GtkWidget * widget, gpointer data);


void dm_clear_track_cb (GtkWidget * widget, gpointer data);
void dm_shift_track_left_cb (GtkWidget * widget, gpointer data);
void dm_shift_track_right_cb (GtkWidget * widget, gpointer data);

guint
drummachine_if_get_type()
{
  static guint b_type = 0;

  if (!b_type) {
    GtkTypeInfo b_info =
    {
      "DrumMachineIF",
      sizeof(DrumMachineIF),
      sizeof(DrumMachineIFClass),
      (GtkClassInitFunc) drummachine_if_class_init,
      (GtkObjectInitFunc) drummachine_if_init,
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

static guint drummachine_if_signals[LAST_SIGNAL+1] =
{0};

static void
drummachine_if_class_init(DrumMachineIFClass * class)
{
  GtkObjectClass *object_class;

  object_class = (GtkObjectClass *) class;

  gtk_object_class_add_signals(object_class, drummachine_if_signals, LAST_SIGNAL);


  class->drummachine_if = NULL;
}

static void
drummachine_if_init(DrumMachineIF * drummachine_if)
{
}

GtkWidget *
drummachine_if_new(drummachine * rsq)
{
  DrumMachineIF *drummachine_if;
  GtkWidget *vbox, *hbox, *hbox2;
  GtkWidget * frame;
  GtkWidget *button, * arrow;
  gint i, j;
  gchar buf[4];

  drummachine_if = gtk_type_new(drummachine_if_get_type());

  drummachine_if->data = rsq;

  gtk_window_set_title(GTK_WINDOW(drummachine_if), ((module *) drummachine_if->data)->u_label);

  gtk_container_border_width(GTK_CONTAINER(drummachine_if), 1);

#if 0
  /*
     when the window is given the "delete_event" signal - this is
     * given by the window manager - usually the close option or on the
     * titlebar - we ask it to call the delete_event() function
     * as defined above. The data passed to the callback function is
     * NULL and is ignored in the callback. 
   */
  gtk_signal_connect(GTK_OBJECT(drummachine_if), "delete_event",
		     GTK_SIGNAL_FUNC(delete_event), NULL);
#endif

#if 1
  /*
     here we connect the "destroy" event to a signal handler.
     * This event occurs when we call gtk_widget_destroy() on the
     * window, or if we return "TRUE" in the "delete_event" callback. 
   */
  gtk_signal_connect(GTK_OBJECT(drummachine_if), "destroy",
		     GTK_SIGNAL_FUNC(drummachine_if_close_cb), drummachine_if);
#endif

  vbox = gtk_vbox_new(FALSE, 5);
  gtk_container_add(GTK_CONTAINER(drummachine_if), vbox);
  gtk_widget_show(vbox);

  hbox2 = gtk_hbox_new(FALSE, 5);
  gtk_box_pack_start(GTK_BOX(vbox), hbox2, FALSE, FALSE, 0);
  gtk_widget_show(hbox2);

  button = gtk_toggle_button_new_with_label("On");
  gtk_box_pack_start(GTK_BOX(hbox2), button, FALSE, FALSE, 1);
  gtk_toggle_button_set_state(GTK_TOGGLE_BUTTON(button),
			      drummachine_if->data->module.on);
  gtk_signal_connect(GTK_OBJECT(button), "clicked",
		     GTK_SIGNAL_FUNC(drummachine_if_onoff_cb),
		     drummachine_if->data);
  gtk_widget_show(button);

  button = opsmenu_new((module *) drummachine_if->data,
		       GTK_WIDGET(drummachine_if),
		       drummachine_if_hide_cb, drummachine_if_close_cb);
  gtk_box_pack_start(GTK_BOX(hbox2), button, FALSE, FALSE, 4);
  gtk_widget_show(button);

  frame = gtk_frame_new (NULL);
  gtk_box_pack_start(GTK_BOX(hbox2), frame, TRUE, TRUE, 1);
  gtk_frame_set_shadow_type (GTK_FRAME (frame), GTK_SHADOW_IN);
  gtk_container_border_width (GTK_CONTAINER (frame), 1);
  gtk_widget_show (frame);

  hbox = gtk_hbox_new (FALSE, 2);
  gtk_container_add (GTK_CONTAINER(frame), hbox);
  gtk_widget_show (hbox);

  button = gtk_label_new ("All tracks:");
  gtk_box_pack_start (GTK_BOX (hbox), button, FALSE, FALSE, 3);
  gtk_widget_show (button);

  button = gtk_button_new_with_label("Clear");
  gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, TRUE, 1);
  gtk_signal_connect(GTK_OBJECT(button), "clicked",
		     GTK_SIGNAL_FUNC(dm_clear_cb), drummachine_if);
  gtk_widget_show(button);

  button = gtk_button_new_with_label("Chaos");
  gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, TRUE, 1);
  gtk_signal_connect(GTK_OBJECT(button), "clicked",
		     GTK_SIGNAL_FUNC(dm_chaos_cb), drummachine_if);
  gtk_widget_show(button);

  button = gtk_button_new ();
  gtk_signal_connect(GTK_OBJECT(button), "clicked",
		     GTK_SIGNAL_FUNC(dm_shift_left_cb),
		     drummachine_if);
  gtk_button_set_relief (GTK_BUTTON(button), GTK_RELIEF_NONE);
  gtk_box_pack_start (GTK_BOX(hbox), button, FALSE, FALSE, 0);
  arrow = gtk_arrow_new (GTK_ARROW_LEFT, GTK_SHADOW_OUT);
  gtk_container_add (GTK_CONTAINER(button), arrow);
  gtk_widget_show (arrow);
  gtk_widget_show (button);
  
  button = gtk_button_new ();
  gtk_signal_connect(GTK_OBJECT(button), "clicked",
		     GTK_SIGNAL_FUNC(dm_shift_right_cb),
		     drummachine_if);
  gtk_button_set_relief (GTK_BUTTON(button), GTK_RELIEF_NONE);
  gtk_box_pack_start (GTK_BOX(hbox), button, FALSE, FALSE, 0);
  arrow = gtk_arrow_new (GTK_ARROW_RIGHT, GTK_SHADOW_OUT);
  gtk_container_add (GTK_CONTAINER(button), arrow);
  gtk_widget_show (arrow);
  gtk_widget_show (button);

  /*
     D R U M  T R A C K S 
   */

  for (j = 0; j < MAX_CHANNELS; j++) {

/*
   if(j%2 == 0) {

   vbox2 = gtk_vbox_new(FALSE, 0);
   gtk_box_pack
 */

    hbox = gtk_hbox_new(FALSE, 2);
    gtk_box_pack_start(GTK_BOX(vbox), hbox, TRUE, TRUE, 1);
    gtk_widget_show(hbox);

    snprintf(buf, 4, "%d:", j);
    button = gtk_label_new(buf);
    gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, FALSE, 3);
    gtk_widget_show(button);

    button = gtk_button_new_with_label ("Clear");
    gtk_object_set_user_data (GTK_OBJECT(button), (gpointer)j);
    gtk_box_pack_start (GTK_BOX(hbox), button, FALSE, FALSE, 0);
    gtk_signal_connect(GTK_OBJECT(button), "clicked",
		       GTK_SIGNAL_FUNC(dm_clear_track_cb), drummachine_if);
    gtk_widget_show (button);

#if 0
    slider = slider_new("Vol", &(drummachine_if->data->vol), 0, 64, 0);
    gtk_box_pack_start(GTK_BOX(hbox), slider, FALSE, FALSE, 0);
    gtk_widget_show(slider);
#endif

    button = gtk_vseparator_new();
    gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, TRUE, 3);
    gtk_widget_show(button);

    for (i = 0; i < DM_LENGTH; i++) {

      if (i && i % 4 == 0) {
	button = gtk_vseparator_new();
	gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, TRUE, 3);
	gtk_widget_show(button);
      }
      button = gtk_toggle_button_new();
      gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, TRUE, 1);
      gtk_widget_set_usize(button, 10, 10);
      gtk_toggle_button_set_state(GTK_TOGGLE_BUTTON(button),
			(drummachine_if->data->sequence[j][i].note == 128));
      gtk_signal_connect(GTK_OBJECT(button), "clicked",
	       GTK_SIGNAL_FUNC(drummachine_if_set_note_cb), drummachine_if);
      gtk_widget_show(button);
      drummachine_if->buttons[j][i].button = button;
    }

    button = gtk_vseparator_new();
    gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, TRUE, 3);
    gtk_widget_show(button);

    button = gtk_button_new ();
    gtk_object_set_user_data (GTK_OBJECT(button), (gpointer)j);
    gtk_signal_connect(GTK_OBJECT(button), "clicked",
		       GTK_SIGNAL_FUNC(dm_shift_track_left_cb),
		       drummachine_if);
    gtk_button_set_relief (GTK_BUTTON(button), GTK_RELIEF_NONE);
    gtk_box_pack_start (GTK_BOX(hbox), button, FALSE, FALSE, 0);
    arrow = gtk_arrow_new (GTK_ARROW_LEFT, GTK_SHADOW_OUT);
    gtk_container_add (GTK_CONTAINER(button), arrow);
    gtk_widget_show (arrow);
    gtk_widget_show (button);

    button = gtk_button_new ();
    gtk_object_set_user_data (GTK_OBJECT(button), (gpointer)j);
    gtk_signal_connect(GTK_OBJECT(button), "clicked",
		       GTK_SIGNAL_FUNC(dm_shift_track_right_cb),
		       drummachine_if);
    gtk_button_set_relief (GTK_BUTTON(button), GTK_RELIEF_NONE);
    gtk_box_pack_start (GTK_BOX(hbox), button, FALSE, FALSE, 0);
    arrow = gtk_arrow_new (GTK_ARROW_RIGHT, GTK_SHADOW_OUT);
    gtk_container_add (GTK_CONTAINER(button), arrow);
    gtk_widget_show (arrow);
    gtk_widget_show (button);

/*
   button = outputlabel_new((module *)drummachine_if->data, j);
   gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 1);
   gtk_widget_show(button);
 */


  }

  return GTK_WIDGET(drummachine_if);
}

void 
drummachine_if_update_dm(GtkWidget * widget, gpointer data)
{
  DrumMachineIF *drummachine_if;
  int i, j;

  drummachine_if = DRUMMACHINE_IF(data);
  for (j = 0; j < MAX_CHANNELS; j++) {
    for (i = 0; i < DM_LENGTH; i++) {
      gtk_signal_handler_block_by_data(GTK_OBJECT(drummachine_if->buttons[j][i].button), drummachine_if);
      gtk_toggle_button_set_state(GTK_TOGGLE_BUTTON(drummachine_if->buttons[j][i].button), (drummachine_if->data->sequence[j][i].note == 128));
      gtk_signal_handler_unblock_by_data(GTK_OBJECT(drummachine_if->buttons[j][i].button), drummachine_if);
    }
  }
}

void 
drummachine_if_hide_cb(GtkWidget * widget, gpointer data)
{
  module *u;
  DrumMachineIF *drummachine_if;

  drummachine_if = DRUMMACHINE_IF(data);
  u = (module *) drummachine_if->data;
  aube_module_remove_if(u);

  gtk_widget_destroy(GTK_WIDGET(data));
}

void 
drummachine_if_close_cb(GtkWidget * widget, gpointer data)
{
  module *u;
  DrumMachineIF *drummachine_if;

  drummachine_if = DRUMMACHINE_IF(data);

  u = (module *) drummachine_if->data;
  aube_remove_module(u);

  free((DRUMMACHINE_IF(data))->data);
  gtk_widget_destroy(GTK_WIDGET(data));
}

void 
drummachine_if_dismiss(GtkWidget * widget, gpointer data)
{
  gtk_widget_destroy(GTK_WIDGET(data));
}

void 
drummachine_if_onoff_cb(GtkWidget * widget, gpointer data)
{
  aube_module_toggle((module *) data);
}

void 
drummachine_if_set_note_cb(GtkWidget * widget, gpointer data)
{
  DrumMachineIF *drummachine_if;
  int i, j;

  drummachine_if = DRUMMACHINE_IF(data);
  for (j = 0; j < MAX_CHANNELS; j++) {
    for (i = 0; i < DM_LENGTH; i++) {
      if (drummachine_if->buttons[j][i].button == widget) {
	drummachine_if->data->sequence[j][i].note = 128 -
	  drummachine_if->data->sequence[j][i].note;
	break;
      }
    }
  }

  drummachine_if_update_dm(NULL, drummachine_if);
}

void 
dm_restart_cb(GtkWidget * widget, gpointer data)
{
  dm_restart((drummachine *) data);
}

void 
dm_clear_cb(GtkWidget * widget, gpointer data)
{
  DrumMachineIF *drummachine_if;

  drummachine_if = DRUMMACHINE_IF(data);
  dm_clear(drummachine_if->data);
  drummachine_if_update_dm(NULL, drummachine_if);
}

void 
dm_chaos_cb(GtkWidget * widget, gpointer data)
{
  DrumMachineIF *drummachine_if;

  drummachine_if = DRUMMACHINE_IF(data);
  dm_chaos(drummachine_if->data);
  drummachine_if_update_dm(NULL, drummachine_if);
}

void
dm_shift_left_cb (GtkWidget * widget, gpointer data)
{
  DrumMachineIF * drummachine_if = DRUMMACHINE_IF (data);
  int j;

  for (j = 0; j < MAX_CHANNELS; j++) {
    dm_shift_track (drummachine_if->data, j, -1);
  }

  drummachine_if_update_dm (NULL, drummachine_if);
}

void
dm_shift_right_cb (GtkWidget * widget, gpointer data)
{
  DrumMachineIF * drummachine_if = DRUMMACHINE_IF (data);
  int j;

  for (j = 0; j < MAX_CHANNELS; j++) {
    dm_shift_track (drummachine_if->data, j, 1);
  }

  drummachine_if_update_dm (NULL, drummachine_if);
}

void
dm_clear_track_cb (GtkWidget * widget, gpointer data)
{
  DrumMachineIF * drummachine_if = DRUMMACHINE_IF (data);
  int j = (int) gtk_object_get_user_data (GTK_OBJECT(widget));

  dm_clear_track (drummachine_if->data, j);

  drummachine_if_update_dm(NULL, drummachine_if);
}

void
dm_shift_track_left_cb (GtkWidget * widget, gpointer data)
{
  DrumMachineIF * drummachine_if = DRUMMACHINE_IF (data);
  int j = (int) gtk_object_get_user_data (GTK_OBJECT(widget));

  dm_shift_track (drummachine_if->data, j, -1);

  drummachine_if_update_dm(NULL, drummachine_if);
}

void
dm_shift_track_right_cb (GtkWidget * widget, gpointer data)
{
  DrumMachineIF * drummachine_if = DRUMMACHINE_IF (data);
  int j = (int) gtk_object_get_user_data (GTK_OBJECT(widget));

  dm_shift_track (drummachine_if->data, j, 1);

  drummachine_if_update_dm(NULL, drummachine_if);
}

void 
dm_play_once_cb(GtkWidget * widget, gpointer data)
{
  DrumMachineIF *drummachine_if;

  drummachine_if = DRUMMACHINE_IF(data);
  dm_play_once(drummachine_if->data);
  drummachine_if_update_dm(NULL, drummachine_if);
}

void 
dm_play_44_cb(GtkWidget * widget, gpointer data)
{
  DrumMachineIF *drummachine_if;

  drummachine_if = DRUMMACHINE_IF(data);
  dm_play_44(drummachine_if->data);
  drummachine_if_update_dm(NULL, drummachine_if);
}

void 
dm_transpose_u12_cb(GtkWidget * widget, gpointer data)
{
  DrumMachineIF *drummachine_if;

  drummachine_if = DRUMMACHINE_IF(data);
  dm_transpose_u12(drummachine_if->data);
  drummachine_if_update_dm(NULL, drummachine_if);
}

void 
dm_transpose_d12_cb(GtkWidget * widget, gpointer data)
{
  DrumMachineIF *drummachine_if;

  drummachine_if = DRUMMACHINE_IF(data);
  dm_transpose_d12(drummachine_if->data);
  drummachine_if_update_dm(NULL, drummachine_if);
}

void 
dm_transpose_u1_cb(GtkWidget * widget, gpointer data)
{
  DrumMachineIF *drummachine_if;

  drummachine_if = DRUMMACHINE_IF(data);
  dm_transpose_u1(drummachine_if->data);
  drummachine_if_update_dm(NULL, drummachine_if);
}

void 
dm_transpose_d1_cb(GtkWidget * widget, gpointer data)
{
  DrumMachineIF *drummachine_if;

  drummachine_if = DRUMMACHINE_IF(data);
  dm_transpose_d1(drummachine_if->data);
  drummachine_if_update_dm(NULL, drummachine_if);
}

void 
dm_lshift_cb(GtkWidget * widget, gpointer data)
{
  dm_shift_left((drummachine *) data);
}

void 
dm_rshift_cb(GtkWidget * widget, gpointer data)
{
  dm_shift_right((drummachine *) data);
}
