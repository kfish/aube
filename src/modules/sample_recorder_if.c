#include <stdlib.h>
#include <strings.h>
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

#include "sample_recorder_if.h"
#include "aube.h"
#include "master.h"
#include "slider.h"
#include "inputoption.h"
#include "outputlabel.h"

extern int tick;

static void sample_recorder_if_class_init(SampleRecorderIFClass * klass);
static void sample_recorder_if_init(SampleRecorderIF * b);
GtkWidget *sample_recorder_if_new(sample_recorder * pn);
void sample_recorder_if_dismiss(GtkWidget * widget, gpointer data);
void sample_recorder_if_hide_cb(GtkWidget * widget, gpointer data);
void sample_recorder_if_close_cb(GtkWidget * widget, gpointer data);
void sample_recorder_if_onoff_cb(GtkWidget * widget, gpointer data);
void sample_recorder_if_record(GtkWidget * widget, gpointer data);
void sample_recorder_if_stop(GtkWidget * widget, gpointer data);
void sample_recorder_if_record_next(GtkWidget * widget, gpointer data);
void sample_recorder_if_set_record_states(SampleRecorderIF * sample_recorder_if);
void sample_recorder_if_passthrough_never(GtkWidget * widget, gpointer data);
void sample_recorder_if_passthrough_rec(GtkWidget * widget, gpointer data);
void sample_recorder_if_passthrough_always(GtkWidget * widget, gpointer data);
void sample_recorder_if_restart(GtkWidget * widget, gpointer data);
void sample_recorder_if_check_record(SampleRecorderIF * SR);
gint sample_recorder_if_update_record(gpointer data);
/*
   void sample_recorder_if_name_changed(GtkWidget *widget, GdkEventKey *event, gpointer data);
 */
void sample_recorder_if_add_sample(GtkWidget * widget, gpointer data);

GType
sample_recorder_if_get_type(void)
{
  static GType b_type = 0;

  if (!b_type) {
    static const GTypeInfo b_info =
    {
      sizeof(SampleRecorderIFClass),
      NULL, /* base_init */
	  NULL, /* base_finalise */
      (GClassInitFunc) sample_recorder_if_class_init,
	  NULL, /* class_finalize */
	  NULL, /* class_data */
      sizeof(SampleRecorderIF),
	  0, /* n_preallocs */
	  (GInstanceInitFunc)sample_recorder_if_init,
    };

    b_type = g_type_register_static(GTK_TYPE_WINDOW,
                                                      "SampleRecorderIF",
	                                                   &b_info, 0);
  }
  return b_type;
}

static void
sample_recorder_if_class_init(SampleRecorderIFClass * klass)
{
 
}

static void
sample_recorder_if_init(SampleRecorderIF * sample_recorder_if)
{
}

GtkWidget *
sample_recorder_if_new(sample_recorder * pn)
{
  SampleRecorderIF *sample_recorder_if;
  GtkWidget *vbox, *vbox2, *hbox, *hbox3;
  GtkWidget *button;
  GtkWidget *notebook, *label;
  GSList *rbgroup;

  sample_recorder_if = gtk_type_new(sample_recorder_if_get_type());

  sample_recorder_if->data = pn;

  sample_recorder_if->recordfunc_tag = 0;

  gtk_window_set_title(GTK_WINDOW(sample_recorder_if), ((module *) sample_recorder_if->data)->u_label);

  gtk_container_border_width(GTK_CONTAINER(sample_recorder_if), 1);

#if 0
  /*
     when the window is given the "delete_event" signal - this is
     * given by the window manager - usually the close option or on the
     * titlebar - we ask it to call the delete_event() function
     * as defined above. The data passed to the callback function is
     * NULL and is ignored in the callback. 
   */
  g_signal_connect(G_OBJECT(sample_recorder_if), "delete_event",
		     G_CALLBACK(delete_event), NULL);
#endif

#if 1
  /*
     here we connect the "destroy" event to a signal handler.
     * This event occurs when we call gtk_widget_destroy() on the
     * window, or if we return "TRUE" in the "delete_event" callback. 
   */
  g_signal_connect(G_OBJECT(sample_recorder_if), "destroy",
		     G_CALLBACK(sample_recorder_if_close_cb), sample_recorder_if);
#endif

  vbox2 = gtk_vbox_new(FALSE, 5);
  gtk_container_add(GTK_CONTAINER(sample_recorder_if), vbox2);
  gtk_widget_show(vbox2);

  /*
     I N P U T 
   */

  hbox = gtk_hbox_new(FALSE, 0);
  gtk_box_pack_start(GTK_BOX(vbox2), hbox, FALSE, FALSE, 0);
  gtk_widget_show(hbox);

  button = gtk_toggle_button_new_with_label("On");
  gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 1);
  gtk_toggle_button_set_state(GTK_TOGGLE_BUTTON(button), sample_recorder_if->data->module.on);
  g_signal_connect(G_OBJECT(button), "clicked",
    G_CALLBACK(sample_recorder_if_onoff_cb), sample_recorder_if->data);
  gtk_widget_show(button);

  button = opsmenu_new((module *) sample_recorder_if->data,
		       GTK_WIDGET(sample_recorder_if),
		       sample_recorder_if_hide_cb,
		       sample_recorder_if_close_cb);
  gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 4);
  gtk_widget_show(button);

  button = outputlabel_new((module *) SAMPLE_RECORDER_IF(sample_recorder_if)->data, 0);
  gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 2);
  gtk_widget_show(button);

  button = inputoption_new((char *) "In:",
		(module *) SAMPLE_RECORDER_IF(sample_recorder_if)->data, 0);
  gtk_box_pack_start(GTK_BOX(vbox2), button, FALSE, TRUE, 2);
  gtk_widget_show(button);

  button = inputoption_new((char *) "Trigger:",
		(module *) SAMPLE_RECORDER_IF(sample_recorder_if)->data, 1);
  gtk_box_pack_start(GTK_BOX(vbox2), button, FALSE, TRUE, 2);
  gtk_widget_show(button);


  /* NOTEBOOK */

  notebook = gtk_notebook_new();
  gtk_box_pack_start(GTK_BOX(vbox2), notebook, TRUE, TRUE, 0);
  gtk_widget_show(notebook);

  hbox = gtk_hbox_new(FALSE, 0);
  label = gtk_label_new("Record");
  gtk_notebook_append_page(GTK_NOTEBOOK(notebook), hbox, label);
  gtk_widget_show(hbox);

  vbox = gtk_vbox_new(FALSE, 5);
  gtk_box_pack_start(GTK_BOX(hbox), vbox, TRUE, TRUE, 0);
  gtk_widget_show(vbox);

  button = gtk_check_button_new_with_label("Record on next trigger");
  gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 2);
  gtk_toggle_button_set_state(GTK_TOGGLE_BUTTON(button),
			      sample_recorder_if->data->record_next);
  g_signal_connect(G_OBJECT(button), "clicked",
       G_CALLBACK(sample_recorder_if_record_next), sample_recorder_if);
  gtk_widget_show(button);
  sample_recorder_if->record_next_check = button;

  hbox3 = gtk_hbox_new(FALSE, 5);
  gtk_box_pack_start(GTK_BOX(vbox), hbox3, TRUE, FALSE, 0);
  gtk_widget_show(hbox3);

  button = gtk_toggle_button_new_with_label("Record");
  gtk_box_pack_start(GTK_BOX(hbox3), button, FALSE, FALSE, 0);
  gtk_toggle_button_set_state(GTK_TOGGLE_BUTTON(button),
			      sample_recorder_if->data->recording);
  g_signal_connect(G_OBJECT(button), "clicked",
	    G_CALLBACK(sample_recorder_if_record), sample_recorder_if);
  gtk_widget_show(button);
  sample_recorder_if->record_toggle = button;

  button = gtk_button_new_with_label("Stop");
  gtk_box_pack_start(GTK_BOX(hbox3), button, FALSE, FALSE, 0);
  g_signal_connect(G_OBJECT(button), "clicked",
	      G_CALLBACK(sample_recorder_if_stop), sample_recorder_if);
  gtk_widget_show(button);

  hbox3 = gtk_hbox_new(FALSE, 5);
  gtk_box_pack_start(GTK_BOX(vbox), hbox3, TRUE, FALSE, 0);
  gtk_widget_show(hbox3);

  button = gtk_progress_bar_new();
  gtk_box_pack_start(GTK_BOX(hbox3), button, FALSE, TRUE, 0);
  gtk_widget_show(button);
  sample_recorder_if->record_pbar = button;

  vbox = gtk_vbox_new(FALSE, 0);
  label = gtk_label_new("Playback");
  gtk_notebook_append_page(GTK_NOTEBOOK(notebook), vbox, label);
  gtk_widget_show(vbox);

  button = gtk_radio_button_new_with_label(NULL, "Always pass input through");
  gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 2);
  gtk_toggle_button_set_state(GTK_TOGGLE_BUTTON(button),
			      (sample_recorder_if->data->passthrough_mode == AUBE_SAMPLE_RECORDER_PASSTHROUGH_ALWAYS));
  g_signal_connect(G_OBJECT(button), "clicked",
		     G_CALLBACK(sample_recorder_if_passthrough_always),
		     sample_recorder_if->data);
  gtk_widget_show(button);

  rbgroup = gtk_radio_button_group(GTK_RADIO_BUTTON(button));

  button = gtk_radio_button_new_with_label(rbgroup, "Pass through when recording,\nelse playback");
  gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 2);
  gtk_toggle_button_set_state(GTK_TOGGLE_BUTTON(button),
			      (sample_recorder_if->data->passthrough_mode == AUBE_SAMPLE_RECORDER_PASSTHROUGH_REC));
  g_signal_connect(G_OBJECT(button), "clicked",
		     G_CALLBACK(sample_recorder_if_passthrough_rec),
		     sample_recorder_if->data);
  gtk_widget_show(button);
  rbgroup = gtk_radio_button_group(GTK_RADIO_BUTTON(button));

  button = gtk_radio_button_new_with_label(rbgroup, "Always playback");
  gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 2);
  gtk_toggle_button_set_state(GTK_TOGGLE_BUTTON(button),
			      (sample_recorder_if->data->passthrough_mode == AUBE_SAMPLE_RECORDER_PASSTHROUGH_NEVER));
  g_signal_connect(G_OBJECT(button), "clicked",
		     G_CALLBACK(sample_recorder_if_passthrough_never),
		     sample_recorder_if->data);
  gtk_widget_show(button);

  button = gtk_button_new_with_label("Restart");
  gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 0);
  g_signal_connect(G_OBJECT(button), "clicked",
		     G_CALLBACK(sample_recorder_if_restart),
		     sample_recorder_if->data);
  gtk_widget_show(button);

  vbox = gtk_vbox_new(FALSE, 0);

  label = gtk_label_new("Buffer");
  gtk_notebook_append_page(GTK_NOTEBOOK(notebook), vbox, label);
  gtk_widget_show(vbox);

#if 0
  hbox3 = gtk_hbox_new(FALSE, 5);
  gtk_box_pack_start(GTK_BOX(vbox), hbox3, TRUE, TRUE, 0);
  gtk_widget_show(hbox3);

  label = gtk_label_new("Length:");
  gtk_box_pack_start(GTK_BOX(hbox3), label, TRUE, TRUE, 0);
  gtk_widget_show(label);

  button = gtk_entry_new();
  gtk_box_pack_start(GTK_BOX(hbox3), button, TRUE, FALSE, 0);
  gtk_widget_show(button);
#endif

  hbox3 = gtk_hbox_new(FALSE, 5);
  gtk_box_pack_start(GTK_BOX(vbox), hbox3, TRUE, FALSE, 0);
  gtk_widget_show(hbox3);

  label = gtk_label_new("Name:");
  gtk_box_pack_start(GTK_BOX(hbox3), label, TRUE, TRUE, 0);
  gtk_widget_show(label);

  button = gtk_entry_new();
  gtk_box_pack_start(GTK_BOX(hbox3), button, TRUE, TRUE, 0);
/*
   g_signal_connect(G_OBJECT(button), "key_press_event",
   G_CALLBACK(sample_recorder_if_name_changed),
   button);
 */
  gtk_widget_show(button);
  sample_recorder_if->samplename_entry = button;

  button = gtk_button_new_with_label("Add to sample list");
  gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 0);
  g_signal_connect(G_OBJECT(button), "clicked",
		     G_CALLBACK(sample_recorder_if_add_sample),
		     sample_recorder_if);
  gtk_widget_show(button);

  return GTK_WIDGET(sample_recorder_if);
}

void 
sample_recorder_if_hide_cb(GtkWidget * widget, gpointer data)
{
  module *u;
  SampleRecorderIF *sample_recorder_if;

  sample_recorder_if = SAMPLE_RECORDER_IF(data);
  u = (module *) sample_recorder_if->data;
  aube_module_remove_if(u);

  gtk_widget_destroy(GTK_WIDGET(data));
}

void 
sample_recorder_if_close_cb(GtkWidget * widget, gpointer data)
{
  module *u;
  SampleRecorderIF *sample_recorder_if;

  sample_recorder_if = SAMPLE_RECORDER_IF(data);
  u = (module *) sample_recorder_if->data;
  aube_remove_module(u);

  free((SAMPLE_RECORDER_IF(data))->data);
  gtk_widget_destroy(GTK_WIDGET(data));
}

void 
sample_recorder_if_dismiss(GtkWidget * widget, gpointer data)
{
  gtk_widget_destroy(GTK_WIDGET(data));
}

void 
sample_recorder_if_onoff_cb(GtkWidget * widget, gpointer data)
{
  aube_module_toggle((module *) data);
}

void 
sample_recorder_if_record(GtkWidget * widget, gpointer data)
{
  SampleRecorderIF *SR = (SampleRecorderIF *) data;
  sample_recorder *sr = SR->data;

  sr->recording = 1 - sr->recording;
  sample_recorder_if_check_record(SR);
}

void 
sample_recorder_if_stop(GtkWidget * widget, gpointer data)
{
  SampleRecorderIF *SR = (SampleRecorderIF *) data;
  sample_recorder *sr = SR->data;

  sample_recorder_stop_recording(sr);
  sample_recorder_if_update_record(SR);
}

void 
sample_recorder_if_record_next(GtkWidget * widget, gpointer data)
{
  SampleRecorderIF *SR = (SampleRecorderIF *) data;
  sample_recorder *sr = SR->data;

  sr->record_next = 1 - sr->record_next;
  sample_recorder_if_check_record(SR);
}

void 
sample_recorder_if_passthrough_never(GtkWidget * widget, gpointer data)
{
  ((sample_recorder *) data)->passthrough_mode =
    AUBE_SAMPLE_RECORDER_PASSTHROUGH_NEVER;
}

void 
sample_recorder_if_passthrough_rec(GtkWidget * widget, gpointer data)
{
  ((sample_recorder *) data)->passthrough_mode =
    AUBE_SAMPLE_RECORDER_PASSTHROUGH_REC;
}

void 
sample_recorder_if_passthrough_always(GtkWidget * widget, gpointer data)
{
  ((sample_recorder *) data)->passthrough_mode =
    AUBE_SAMPLE_RECORDER_PASSTHROUGH_ALWAYS;
}

void 
sample_recorder_if_restart(GtkWidget * widget, gpointer data)
{
  ((sample_recorder *) data)->play_offset = 0;
}

void 
sample_recorder_if_check_record(SampleRecorderIF * SR)
{
  sample_recorder *sr = SR->data;

  if (!sr->recording && !sr->record_next) {
    gtk_idle_remove(SR->recordfunc_tag);
    SR->recordfunc_tag = 0;
  } else if (!SR->recordfunc_tag) {
    SR->recordfunc_tag =
      gtk_idle_add((GtkFunction) sample_recorder_if_update_record, SR);
  }
}

gint 
sample_recorder_if_update_record(gpointer data)
{
  SampleRecorderIF *SR = (SampleRecorderIF *) data;
  gfloat percent;


  g_signal_handlers_block_matched (SR->record_next_check, G_SIGNAL_MATCH_DATA, 0, 0, NULL, NULL, SR);
  gtk_toggle_button_set_state(GTK_TOGGLE_BUTTON(GTK_CHECK_BUTTON(SR->record_next_check)), SR->data->record_next);
  g_signal_handlers_unblock_matched (SR->record_next_check, G_SIGNAL_MATCH_DATA, 0, 0, NULL, NULL, SR);


  g_signal_handlers_block_matched (SR->record_toggle, G_SIGNAL_MATCH_DATA, 0, 0, NULL, NULL, SR);
  gtk_toggle_button_set_state(GTK_TOGGLE_BUTTON(SR->record_toggle),
			      SR->data->recording);
  g_signal_handlers_unblock_matched (SR->record_toggle, G_SIGNAL_MATCH_DATA, 0, 0, NULL, NULL, SR);

  if (!SR->data->recording && !SR->data->record_next) {
    gtk_idle_remove(SR->recordfunc_tag);
    SR->recordfunc_tag = 0;
    return 0;
  }
  if (SR->data->recording) {
    percent = (gfloat) (SR->data->rec_offset) / (gfloat) (AUBE_SR_BUFFER_TICKS * DEFAULT_TICK);
    gtk_progress_bar_update(GTK_PROGRESS_BAR(SR->record_pbar), percent);
  }

  return 1;
}

/*
   void sample_recorder_if_name_changed(GtkWidget *widget, GdkEventKey *event, gpointer data)
   {
   if(event->keyval == GDK_Return) {
   printf("%s\n", gtk_entry_get_text(GTK_ENTRY(data)));
   }
   }
 */

void 
sample_recorder_if_add_sample(GtkWidget * widget, gpointer data)
{
  SampleRecorderIF *SR = (SampleRecorderIF *) data;
  sample_recorder *sr = SR->data;

  sample_recorder_add_sample(sr, gtk_entry_get_text(GTK_ENTRY(SR->samplename_entry)));
}
