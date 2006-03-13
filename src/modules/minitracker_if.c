#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <gtk/gtk.h>
#include "aube.h"
#include "master.h"
#include "opsmenu.h"
#include "outputlabel.h"
#include "minitracker_if.h"
#include "slider.h"


extern minitracker *minitracker_new(void);

extern int tick;
extern char *note_names[97];

guint minitracker_if_get_type(void);
static void minitracker_if_class_init(MinitrackerIFClass * class);
static void minitracker_if_init(MinitrackerIF * b);
GtkWidget *minitracker_if_new(minitracker * rsq);
void minitracker_if_dismiss(GtkWidget * widget, gpointer data);

void minitracker_if_update_tracker(GtkWidget * widget, gpointer data);
void minitracker_if_hide_cb(GtkWidget * widget, gpointer data);
void minitracker_if_close_cb(GtkWidget * widget, gpointer data);
void minitracker_if_onoff_cb(GtkWidget * widget, gpointer data);
void seq_toggle_cb(GtkWidget * widget, gpointer data);
void restart_cb(GtkWidget * widget, gpointer data);
void chaos_cb(GtkWidget * widget, gpointer data);
void seq_chaos8_cb(GtkWidget * widget, gpointer data);
void seq_play_once_cb(GtkWidget * widget, gpointer data);
void seq_play_44_cb(GtkWidget * widget, gpointer data);
void seq_transpose_u12_cb(GtkWidget * widget, gpointer data);
void seq_transpose_d12_cb(GtkWidget * widget, gpointer data);
void seq_transpose_u1_cb(GtkWidget * widget, gpointer data);
void seq_transpose_d1_cb(GtkWidget * widget, gpointer data);
void lshift_cb(GtkWidget * widget, gpointer data);
void rshift_cb(GtkWidget * widget, gpointer data);

guint
minitracker_if_get_type()
{
  static guint b_type = 0;

  if (!b_type) {
    GtkTypeInfo b_info =
    {
      "MinitrackerIF",
      sizeof(MinitrackerIF),
      sizeof(MinitrackerIFClass),
      (GtkClassInitFunc) minitracker_if_class_init,
      (GtkObjectInitFunc) minitracker_if_init,
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

static gint minitracker_if_signals[LAST_SIGNAL] =
{0};

static void
minitracker_if_class_init(MinitrackerIFClass * class)
{
  GtkObjectClass *object_class;

  object_class = (GtkObjectClass *) class;

  gtk_object_class_add_signals(object_class, minitracker_if_signals, LAST_SIGNAL);


  class->minitracker_if = NULL;
}

static void
minitracker_if_init(MinitrackerIF * minitracker_if)
{
}

GtkWidget *
minitracker_if_new(minitracker * rsq)
{
  MinitrackerIF *minitracker_if;
  GtkWidget *vbox, *vbox2, *hbox, *hbox2, *hbox3;
  GtkWidget *frame;
  GtkWidget *button;
  GtkWidget *slider;
  GSList *rbgroup;
  GtkWidget * scrolled;
  gchar *titles[] =
  {"", "Note", "Vol", "Pan"};

  minitracker_if = gtk_type_new(minitracker_if_get_type());

  minitracker_if->data = rsq;

  gtk_window_set_title(GTK_WINDOW(minitracker_if), ((module *) minitracker_if->data)->u_label);

  gtk_container_border_width(GTK_CONTAINER(minitracker_if), 1);

#if 0
  /*
     when the window is given the "delete_event" signal - this is
     * given by the window manager - usually the close option or on the
     * titlebar - we ask it to call the delete_event() function
     * as defined above. The data passed to the callback function is
     * NULL and is ignored in the callback. 
   */
  gtk_signal_connect(GTK_OBJECT(minitracker_if), "delete_event",
		     GTK_SIGNAL_FUNC(delete_event), NULL);
#endif

#if 1
  /*
     here we connect the "destroy" event to a signal handler.
     * This event occurs when we call gtk_widget_destroy() on the
     * window, or if we return "TRUE" in the "delete_event" callback. 
   */
  gtk_signal_connect(GTK_OBJECT(minitracker_if), "destroy",
		     GTK_SIGNAL_FUNC(minitracker_if_close_cb), minitracker_if);
#endif

  vbox2 = gtk_vbox_new(FALSE, 5);
  gtk_container_add(GTK_CONTAINER(minitracker_if), vbox2);
  gtk_widget_show(vbox2);

  hbox2 = gtk_hbox_new(FALSE, 5);
  gtk_box_pack_start(GTK_BOX(vbox2), hbox2, FALSE, FALSE, 0);
  gtk_widget_show(hbox2);

  button = gtk_toggle_button_new_with_label("On");
  gtk_box_pack_start(GTK_BOX(hbox2), button, FALSE, FALSE, 1);
  gtk_toggle_button_set_state(GTK_TOGGLE_BUTTON(button), minitracker_if->data->module.on);
  gtk_signal_connect(GTK_OBJECT(button), "clicked",
	    GTK_SIGNAL_FUNC(minitracker_if_onoff_cb), minitracker_if->data);
  gtk_widget_show(button);

  button = opsmenu_new((module *) minitracker_if->data, GTK_WIDGET(minitracker_if),
		       minitracker_if_hide_cb, minitracker_if_close_cb);
  gtk_box_pack_start(GTK_BOX(hbox2), button, FALSE, FALSE, 4);
  gtk_widget_show(button);

  button = outputlabel_new((module *) minitracker_if->data, 0);
  gtk_box_pack_start(GTK_BOX(hbox2), button, FALSE, FALSE, 4);
  gtk_widget_show(button);

  hbox = gtk_hbox_new(FALSE, 5);
  gtk_box_pack_start(GTK_BOX(vbox2), hbox, FALSE, FALSE, 0);
  gtk_widget_show(hbox);



  frame = gtk_frame_new(NULL);
  gtk_box_pack_start(GTK_BOX(hbox), frame, FALSE, FALSE, 0);
/*
   gtk_container_border_width(GTK_CONTAINER(frame), 4);
 */
  gtk_widget_show(frame);

  hbox2 = gtk_hbox_new(FALSE, 0);
  gtk_container_add(GTK_CONTAINER(frame), hbox2);
  gtk_widget_show(hbox2);

  vbox = gtk_vbox_new(FALSE, 0);
  gtk_box_pack_start(GTK_BOX(hbox2), vbox, FALSE, FALSE, 0);
  gtk_widget_show(vbox);

  hbox3 = gtk_hbox_new(FALSE, 0);
  gtk_box_pack_start(GTK_BOX(vbox), hbox3, FALSE, FALSE, 0);
  gtk_widget_show(hbox3);

  slider = slider_new("Vol", &(minitracker_if->data->vol), 0, 64, 1);
  gtk_box_pack_start(GTK_BOX(hbox3), slider, FALSE, FALSE, 0);
  gtk_widget_show(slider);

  slider = slider_new("Tune", &(minitracker_if->data->tune), 1, 1024, 1);
  gtk_box_pack_start(GTK_BOX(hbox3), slider, FALSE, FALSE, 0);
  gtk_widget_show(slider);

  slider = slider_new("Pan", &(minitracker_if->data->pan), 0, 32, 0);
  gtk_box_pack_start(GTK_BOX(vbox), slider, FALSE, FALSE, 0);
  gtk_widget_show(slider);


  vbox = gtk_vbox_new(FALSE, 2);
  gtk_box_pack_start(GTK_BOX(hbox), vbox, FALSE, FALSE, 1);
  gtk_widget_show(vbox);

  button = gtk_radio_button_new_with_label(NULL, "One note");
  gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 0);
  gtk_signal_connect(GTK_OBJECT(button), "clicked",
		     GTK_SIGNAL_FUNC(seq_play_once_cb), minitracker_if);
  gtk_widget_show(button);
  rbgroup = gtk_radio_button_group(GTK_RADIO_BUTTON(button));

  button = gtk_radio_button_new_with_label(rbgroup, "Straight 4/4");
  gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 0);
  gtk_signal_connect(GTK_OBJECT(button), "clicked",
		     GTK_SIGNAL_FUNC(seq_play_44_cb), minitracker_if);
  gtk_widget_show(button);
  rbgroup = gtk_radio_button_group(GTK_RADIO_BUTTON(button));

  button = gtk_radio_button_new_with_label(rbgroup, "Random 8/8");
  gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 0);
  gtk_signal_connect(GTK_OBJECT(button), "clicked",
		     GTK_SIGNAL_FUNC(seq_chaos8_cb), minitracker_if);
  gtk_widget_show(button);
  rbgroup = gtk_radio_button_group(GTK_RADIO_BUTTON(button));

  button = gtk_radio_button_new_with_label(rbgroup, "Random 16/16");
  gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 0);
  gtk_signal_connect(GTK_OBJECT(button), "clicked",
		     GTK_SIGNAL_FUNC(chaos_cb), minitracker_if);
  gtk_widget_show(button);

  vbox = gtk_vbox_new(FALSE, 2);
  gtk_box_pack_start(GTK_BOX(hbox), vbox, FALSE, FALSE, 0);
  gtk_widget_show(vbox);

  hbox3 = gtk_hbox_new(FALSE, 2);
  gtk_box_pack_start(GTK_BOX(vbox), hbox3, FALSE, FALSE, 0);
  gtk_widget_show(hbox3);

  button = gtk_button_new_with_label("<<");
  gtk_box_pack_start(GTK_BOX(hbox3), button, FALSE, FALSE, 0);
  gtk_signal_connect(GTK_OBJECT(button), "clicked",
		     GTK_SIGNAL_FUNC(lshift_cb), minitracker_if);
  gtk_widget_show(button);

  button = gtk_button_new_with_label("Restart");
  gtk_box_pack_start(GTK_BOX(hbox3), button, FALSE, FALSE, 0);
  gtk_signal_connect(GTK_OBJECT(button), "clicked",
		     GTK_SIGNAL_FUNC(restart_cb), minitracker_if);
  gtk_widget_show(button);

  button = gtk_button_new_with_label(">>");
  gtk_box_pack_start(GTK_BOX(hbox3), button, FALSE, FALSE, 0);
  gtk_signal_connect(GTK_OBJECT(button), "clicked",
		     GTK_SIGNAL_FUNC(rshift_cb), minitracker_if);
  gtk_widget_show(button);

  hbox3 = gtk_hbox_new(FALSE, 2);
  gtk_box_pack_start(GTK_BOX(vbox), hbox3, TRUE, FALSE, 0);
  gtk_widget_show(hbox3);

  button = gtk_button_new_with_label("-1");
  gtk_box_pack_start(GTK_BOX(hbox3), button, FALSE, FALSE, 0);
  gtk_signal_connect(GTK_OBJECT(button), "clicked",
		     GTK_SIGNAL_FUNC(seq_transpose_d1_cb), minitracker_if);
  gtk_widget_show(button);

  button = gtk_button_new_with_label("+1");
  gtk_box_pack_start(GTK_BOX(hbox3), button, FALSE, FALSE, 0);
  gtk_signal_connect(GTK_OBJECT(button), "clicked",
		     GTK_SIGNAL_FUNC(seq_transpose_u1_cb), minitracker_if);
  gtk_widget_show(button);

  hbox3 = gtk_hbox_new(FALSE, 2);
  gtk_box_pack_start(GTK_BOX(vbox), hbox3, FALSE, FALSE, 0);
  gtk_widget_show(hbox3);

  button = gtk_button_new_with_label("-12");
  gtk_box_pack_start(GTK_BOX(hbox3), button, FALSE, FALSE, 0);
  gtk_signal_connect(GTK_OBJECT(button), "clicked",
		     GTK_SIGNAL_FUNC(seq_transpose_d12_cb), minitracker_if);
  gtk_widget_show(button);

  button = gtk_button_new_with_label("+12");
  gtk_box_pack_start(GTK_BOX(hbox3), button, FALSE, FALSE, 0);
  gtk_signal_connect(GTK_OBJECT(button), "clicked",
		     GTK_SIGNAL_FUNC(seq_transpose_u12_cb), minitracker_if);
  gtk_widget_show(button);


  /*
     P O R T A M E N T O 
   */

  slider = slider_new("Portamento", &(minitracker_if->data->portamento), 0, 64, 1);
  gtk_box_pack_start(GTK_BOX(hbox), slider, TRUE, FALSE, 0);
  gtk_widget_show(slider);

  /*
     T R A C K E R 
   */

  scrolled = gtk_scrolled_window_new (NULL, NULL);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled),
				  GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
  gtk_box_pack_start(GTK_BOX(vbox2), scrolled, TRUE, TRUE, 1);
  gtk_widget_show (scrolled);

  minitracker_if->tl = gtk_clist_new_with_titles(4, titles);
  gtk_clist_column_titles_passive(GTK_CLIST(minitracker_if->tl));
  gtk_clist_set_selection_mode(GTK_CLIST(minitracker_if->tl),
			       GTK_SELECTION_SINGLE);
  /*
     gtk_clist_set_policy(GTK_CLIST(minitracker_if->tl), GTK_POLICY_AUTOMATIC,
     GTK_POLICY_AUTOMATIC);
   */
  /*
     gtk_clist_set_border(GTK_CLIST(minitracker_if->tl), GTK_SHADOW_IN);
   */
  gtk_clist_set_column_width(GTK_CLIST(minitracker_if->tl), 0, 12);
  gtk_clist_set_column_width(GTK_CLIST(minitracker_if->tl), 1, 100);
  gtk_clist_set_column_width(GTK_CLIST(minitracker_if->tl), 2, 20);
  gtk_clist_set_column_width(GTK_CLIST(minitracker_if->tl), 3, 20);
  gtk_clist_set_column_justification(GTK_CLIST(minitracker_if->tl), 1,
				     GTK_JUSTIFY_CENTER);
  gtk_container_add (GTK_CONTAINER (scrolled), minitracker_if->tl);


  minitracker_if_update_tracker(NULL, minitracker_if);

  gtk_widget_show(minitracker_if->tl);

  return GTK_WIDGET(minitracker_if);
}

void 
minitracker_if_update_tracker(GtkWidget * widget, gpointer data)
{
  MinitrackerIF *tracker;
  gchar *title[4];
  gchar buf0[4];
  gchar buf1[8];
  gchar buf2[4];
  gchar buf3[4];
  int i;

  tracker = MINITRACKER_IF(data);
  gtk_clist_freeze(GTK_CLIST(tracker->tl));
  gtk_clist_clear(GTK_CLIST(tracker->tl));
  title[0] = buf0;
  title[1] = buf1;
  title[2] = buf2;
  title[3] = buf3;
  for (i = 0; i < SEQ_LENGTH; i++) {
    sprintf(title[0], "%02d", i);
    /*    sprintf(title[1], "%d", tracker->data->sequence[i].note);*/
    sprintf (title[1], "%s", note_names[tracker->data->sequence[i].note]);
    sprintf(title[2], "%02d", 0);
    sprintf(title[3], "%02d", 0);

    gtk_clist_append(GTK_CLIST(tracker->tl), (gchar **) & title);
  }
  gtk_clist_thaw(GTK_CLIST(tracker->tl));
}

void 
minitracker_if_hide_cb(GtkWidget * widget, gpointer data)
{
  module *u;
  MinitrackerIF *minitracker_if;

  minitracker_if = MINITRACKER_IF(data);
  u = (module *) minitracker_if->data;
  aube_module_remove_if(u);

  gtk_widget_destroy(GTK_WIDGET(data));
}

void 
minitracker_if_close_cb(GtkWidget * widget, gpointer data)
{
  module *u;
  MinitrackerIF *minitracker_if;

  minitracker_if = MINITRACKER_IF(data);

  u = (module *) minitracker_if->data;
  aube_remove_module(u);

  free((MINITRACKER_IF(data))->data);
  gtk_widget_destroy(GTK_WIDGET(data));
}

void 
minitracker_if_dismiss(GtkWidget * widget, gpointer data)
{
  gtk_widget_destroy(GTK_WIDGET(data));
}

void 
minitracker_if_onoff_cb(GtkWidget * widget, gpointer data)
{
  aube_module_toggle((module *) data);
}

void 
seq_toggle_cb(GtkWidget * widget, gpointer data)
{
  seq_toggle((minitracker *) data);
}

void 
restart_cb(GtkWidget * widget, gpointer data)
{
  seq_restart((minitracker *) data);
}

void 
chaos_cb(GtkWidget * widget, gpointer data)
{
  MinitrackerIF *minitracker_if;

  minitracker_if = MINITRACKER_IF(data);
  chaos(minitracker_if->data);
  minitracker_if_update_tracker(NULL, minitracker_if);
}

void 
seq_chaos8_cb(GtkWidget * widget, gpointer data)
{
  MinitrackerIF *minitracker_if;

  minitracker_if = MINITRACKER_IF(data);
  seq_chaos8(minitracker_if->data);
  minitracker_if_update_tracker(NULL, minitracker_if);
}

void 
seq_play_once_cb(GtkWidget * widget, gpointer data)
{
  MinitrackerIF *minitracker_if;

  minitracker_if = MINITRACKER_IF(data);
  seq_play_once(minitracker_if->data);
  minitracker_if_update_tracker(NULL, minitracker_if);
}

void 
seq_play_44_cb(GtkWidget * widget, gpointer data)
{
  MinitrackerIF *minitracker_if;

  minitracker_if = MINITRACKER_IF(data);
  seq_play_44(minitracker_if->data);
  minitracker_if_update_tracker(NULL, minitracker_if);
}

void 
seq_transpose_u12_cb(GtkWidget * widget, gpointer data)
{
  MinitrackerIF *minitracker_if;

  minitracker_if = MINITRACKER_IF(data);
  seq_transpose_u12(minitracker_if->data);
  minitracker_if_update_tracker(NULL, minitracker_if);
}

void 
seq_transpose_d12_cb(GtkWidget * widget, gpointer data)
{
  MinitrackerIF *minitracker_if;

  minitracker_if = MINITRACKER_IF(data);
  seq_transpose_d12(minitracker_if->data);
  minitracker_if_update_tracker(NULL, minitracker_if);
}

void 
seq_transpose_u1_cb(GtkWidget * widget, gpointer data)
{
  MinitrackerIF *minitracker_if;

  minitracker_if = MINITRACKER_IF(data);
  seq_transpose_u1(minitracker_if->data);
  minitracker_if_update_tracker(NULL, minitracker_if);
}

void 
seq_transpose_d1_cb(GtkWidget * widget, gpointer data)
{
  MinitrackerIF *minitracker_if;

  minitracker_if = MINITRACKER_IF(data);
  seq_transpose_d1(minitracker_if->data);
  minitracker_if_update_tracker(NULL, minitracker_if);
}

void 
lshift_cb(GtkWidget * widget, gpointer data)
{
  shift_left((minitracker *) data);
}

void 
rshift_cb(GtkWidget * widget, gpointer data)
{
  shift_right((minitracker *) data);
}
