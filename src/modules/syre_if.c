#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <gtk/gtk.h>
#include "aube.h"
#include "master.h"
#include "opsmenu.h"
#include "syre_if.h"
#include "slider.h"
#include "inputoption.h"
#include "outputlabel.h"

extern bit16 sinwave[SINTAB_LEN];
extern bit16 squarewave[SINTAB_LEN];
extern bit16 sawtooth[SINTAB_LEN];
extern bit16 wnwave[SINTAB_LEN];

extern int tick;

static void syre_if_class_init(SyreIFClass * klass);
static void syre_if_init(SyreIF * b);
GtkWidget *syre_if_new(syre_if_data * syre_if_data);
void syre_if_dismiss(GtkWidget * widget, gpointer data);

void refresh_sliders(SyreIF * syre_if);
void syre_if_hide_cb(GtkWidget * widget, gpointer data);
void syre_if_close_cb(GtkWidget * widget, gpointer data);
void startstop_cb(GtkWidget * widget, gpointer data);
void copy_accented_cb(GtkWidget * widget, gpointer data);
void copy_unaccented_cb(GtkWidget * widget, gpointer data);
void chaos_harmonics_cb(GtkWidget * widget, gpointer data);
void clear_harmonics_cb(GtkWidget * widget, gpointer data);
void clear_odd_harmonics_cb(GtkWidget * widget, gpointer data);
void harmonics_changed_cb(GtkWidget * widget, gpointer data);
gint set_sinwave_cb(GtkWidget * widget, gpointer * data);
gint set_squarewave_cb(GtkWidget * widget, gpointer * data);
gint set_sawtooth_cb(GtkWidget * widget, gpointer * data);
gint set_wnwave_cb(GtkWidget * widget, gpointer * data);
gint get_envelopes(gpointer data);

GType
syre_if_get_type(void)
{
  static GType b_type = 0;

  if (!b_type) {
    static const GTypeInfo b_info =
    {
      sizeof(SyreIFClass),
      NULL, /* base_init */
	  NULL, /* base_finalise */
      (GClassInitFunc) syre_if_class_init,
	  NULL, /* class_finalize */
	  NULL, /* class_data */
      sizeof(SyreIF),
	  0, /* n_preallocs */
	  (GInstanceInitFunc) syre_if_init,
    };

    b_type = g_type_register_static(GTK_TYPE_WINDOW,
                                                      "SyreIF",
	                                                   &b_info, 0);
  }
  return b_type;
}

static void
syre_if_class_init(SyreIFClass * klass)
{
 
}

static void
syre_if_init(SyreIF * syre_if)
{
}

GtkWidget *
syre_if_new(syre_if_data * sd)
{
  SyreIF *syre_if;
  GtkWidget *vbox, *vbox2, *hbox, *hbox2, *hbox3;
  GtkWidget *notebook;
  GtkWidget *label;
  GtkWidget *frame;
  GtkWidget *button;
  GSList *rbgroup;

  int i;
  char nc[3];

  syre_if = gtk_type_new(syre_if_get_type());

  syre_if->data = sd;

  gtk_window_set_title(GTK_WINDOW(syre_if), ((module *) syre_if->data)->u_label);

  gtk_container_border_width(GTK_CONTAINER(syre_if), 1);

#if 0
  /*
     when the window is given the "delete_event" signal - this is
     * given by the window manager - usually the close option or on the
     * titlebar - we ask it to call the delete_event() function
     * as defined above. The data passed to the callback function is
     * NULL and is ignored in the callback. 
   */
  gtk_signal_connect(GTK_OBJECT(syre_if), "delete_event",
		     GTK_SIGNAL_FUNC(delete_event), NULL);
#endif

#if 1
  /*
     here we connect the "destroy" event to a signal handler.
     * This event occurs when we call gtk_widget_destroy() on the
     * window, or if we return "TRUE" in the "delete_event" callback. 
   */
  gtk_signal_connect(GTK_OBJECT(syre_if), "destroy",
		     GTK_SIGNAL_FUNC(syre_if_close_cb), syre_if);
#endif

  vbox2 = gtk_vbox_new(FALSE, 5);
  gtk_container_add(GTK_CONTAINER(syre_if), vbox2);
  gtk_widget_show(vbox2);


  /*
     S Y N T H 
   */

  hbox = gtk_hbox_new(FALSE, 0);
  gtk_box_pack_start(GTK_BOX(vbox2), hbox, FALSE, FALSE, 0);
  gtk_widget_show(hbox);

  button = gtk_toggle_button_new_with_label("On");
  gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 1);
  gtk_toggle_button_set_state(GTK_TOGGLE_BUTTON(button), syre_if->data->module.on);
  gtk_signal_connect(GTK_OBJECT(button), "clicked",
		     GTK_SIGNAL_FUNC(startstop_cb), syre_if->data);
  gtk_widget_show(button);

  button = opsmenu_new((module *) syre_if->data, GTK_WIDGET(syre_if), syre_if_hide_cb,
		       syre_if_close_cb);
  gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 4);
  gtk_widget_show(button);

  button = inputoption_new((char *) "Sequencer:", (module *) SYRE_IF(syre_if)->data, 0);
  gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 2);
  gtk_widget_show(button);

  button = outputlabel_new((module *) SYRE_IF(syre_if)->data, 0);
  gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 2);
  gtk_widget_show(button);

  hbox2 = gtk_hbox_new(FALSE, 5);
  gtk_box_pack_start(GTK_BOX(vbox2), hbox2, TRUE, TRUE, 0);
  gtk_widget_show(hbox2);

  /*
     NOTEBOOK 
   */

  notebook = gtk_notebook_new();
  gtk_box_pack_start(GTK_BOX(hbox2), notebook, TRUE, TRUE, 0);
  gtk_widget_show(notebook);


  /*
     U N A C C E N T E D 
   */

  hbox = gtk_hbox_new(FALSE, 0);
  label = gtk_label_new("Unaccented");
  gtk_notebook_append_page(GTK_NOTEBOOK(notebook), hbox, label);
  gtk_widget_show(hbox);

  vbox = gtk_vbox_new(FALSE, 5);
  gtk_box_pack_start(GTK_BOX(hbox), vbox, FALSE, FALSE, 0);
  gtk_widget_show(vbox);

  button = gtk_button_new_with_label("Copy\nAccented");
  gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 0);
  gtk_signal_connect(GTK_OBJECT(button), "clicked",
		     GTK_SIGNAL_FUNC(copy_accented_cb), syre_if);
  gtk_widget_show(button);

  /*
     AMPLITUDE ENVELOPE 
   */

  frame = gtk_frame_new("Amplitude Envelope");
  gtk_box_pack_start(GTK_BOX(hbox), frame, FALSE, FALSE, 0);
  gtk_container_border_width(GTK_CONTAINER(frame), 4);
  gtk_widget_show(frame);

  syre_if->amp_env_curve = gtk_gamma_curve_new();
  gtk_container_add(GTK_CONTAINER(frame), syre_if->amp_env_curve);
  gtk_curve_set_range(GTK_CURVE(GTK_GAMMA_CURVE(syre_if->amp_env_curve)->curve),
		      0, ENVELOPE_LEN, 0, 64);
  gtk_curve_set_vector(GTK_CURVE(GTK_GAMMA_CURVE(syre_if->amp_env_curve)->curve),
		       ENVELOPE_LEN, syre_if->data->amp_envelope);
  /*
     gtk_curve_set_curve_type(GTK_CURVE(GTK_GAMMA_CURVE(syre_if->amp_env_curve)->curve),
     GTK_CURVE_TYPE_LINEAR);
   */
  gtk_widget_show(syre_if->amp_env_curve);


  /*
     PITCH ENVELOPE 
   */

  frame = gtk_frame_new("Pitch Envelope");
  gtk_box_pack_start(GTK_BOX(hbox), frame, FALSE, TRUE, 0);
  gtk_container_border_width(GTK_CONTAINER(frame), 4);
  gtk_widget_show(frame);

  syre_if->pitch_env_curve = gtk_gamma_curve_new();
  gtk_container_add(GTK_CONTAINER(frame), syre_if->pitch_env_curve);
  gtk_curve_set_range(GTK_CURVE(GTK_GAMMA_CURVE(syre_if->pitch_env_curve)->curve),
		      0, ENVELOPE_LEN, 0, 64);
  gtk_curve_set_vector(GTK_CURVE(GTK_GAMMA_CURVE(syre_if->pitch_env_curve)->curve),
		       ENVELOPE_LEN, syre_if->data->pitch_envelope);
  /*
     gtk_curve_set_curve_type(GTK_CURVE(GTK_GAMMA_CURVE(syre_if->pitch_env_curve)->curve),
     GTK_CURVE_TYPE_LINEAR);
   */
  gtk_widget_show(syre_if->pitch_env_curve);

  /*
     A C C E N T E D 
   */

  hbox = gtk_hbox_new(FALSE, 0);
  label = gtk_label_new("Accented");
  gtk_notebook_append_page(GTK_NOTEBOOK(notebook), hbox, label);
  gtk_widget_show(hbox);

  vbox = gtk_vbox_new(FALSE, 5);
  gtk_box_pack_start(GTK_BOX(hbox), vbox, FALSE, FALSE, 0);
  gtk_widget_show(vbox);

  button = gtk_button_new_with_label("Copy\nUnaccented");
  gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 0);
  gtk_signal_connect(GTK_OBJECT(button), "clicked",
		     GTK_SIGNAL_FUNC(copy_unaccented_cb), syre_if);
  gtk_widget_show(button);

  /*
     AMPLITUDE ENVELOPE 
   */

  frame = gtk_frame_new("Amplitude Envelope");
  gtk_box_pack_start(GTK_BOX(hbox), frame, FALSE, FALSE, 0);
  gtk_container_border_width(GTK_CONTAINER(frame), 4);
  gtk_widget_show(frame);

  syre_if->amp_env_curve_accented = gtk_gamma_curve_new();
  gtk_container_add(GTK_CONTAINER(frame), syre_if->amp_env_curve_accented);
  gtk_curve_set_range(GTK_CURVE(GTK_GAMMA_CURVE(syre_if->amp_env_curve_accented)->curve),
		      0, ENVELOPE_LEN, 0, 64);
  gtk_curve_set_vector(GTK_CURVE(GTK_GAMMA_CURVE(syre_if->amp_env_curve_accented)->curve),
		       ENVELOPE_LEN, syre_if->data->amp_envelope_accented);
  /*
     gtk_curve_set_curve_type(GTK_CURVE(GTK_GAMMA_CURVE(syre_if->amp_env_curve_accented)->curve),
     GTK_CURVE_TYPE_LINEAR);
   */
  gtk_widget_show(syre_if->amp_env_curve_accented);


  /*
     PITCH ENVELOPE 
   */

  frame = gtk_frame_new("Pitch Envelope");
  gtk_box_pack_start(GTK_BOX(hbox), frame, FALSE, TRUE, 0);
  gtk_container_border_width(GTK_CONTAINER(frame), 4);
  gtk_widget_show(frame);

  syre_if->pitch_env_curve_accented = gtk_gamma_curve_new();
  gtk_container_add(GTK_CONTAINER(frame), syre_if->pitch_env_curve_accented);
  gtk_curve_set_range(GTK_CURVE(GTK_GAMMA_CURVE(syre_if->pitch_env_curve_accented)->curve),
		      0, ENVELOPE_LEN, 0, 64);
  gtk_curve_set_vector(GTK_CURVE(GTK_GAMMA_CURVE(syre_if->pitch_env_curve_accented)->curve),
		       ENVELOPE_LEN, syre_if->data->pitch_envelope_accented);
  /*
     gtk_curve_set_curve_type(GTK_CURVE(GTK_GAMMA_CURVE(syre_if->pitch_env_curve_accented)->curve),
     GTK_CURVE_TYPE_LINEAR);
   */
  gtk_widget_show(syre_if->pitch_env_curve_accented);

  /*
     H A R M O N I C S 
   */

  hbox = gtk_hbox_new(FALSE, 0);
  label = gtk_label_new("Harmonics");
  gtk_notebook_append_page(GTK_NOTEBOOK(notebook), hbox, label);
  gtk_widget_show(hbox);

  frame = gtk_frame_new("Presets");
  gtk_box_pack_start(GTK_BOX(hbox), frame, TRUE, TRUE, 0);
  gtk_container_border_width(GTK_CONTAINER(frame), 2);
  gtk_widget_show(frame);

  vbox = gtk_vbox_new(TRUE, 5);
  gtk_container_add(GTK_CONTAINER(frame), vbox);
  gtk_widget_show(vbox);

  button = gtk_button_new_with_label("Clear");
  gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 0);
  gtk_signal_connect(GTK_OBJECT(button), "clicked",
		     GTK_SIGNAL_FUNC(clear_harmonics_cb), syre_if);
  gtk_widget_show(button);

  button = gtk_button_new_with_label("Clear Odds");
  gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 0);
  gtk_signal_connect(GTK_OBJECT(button), "clicked",
		     GTK_SIGNAL_FUNC(clear_odd_harmonics_cb), syre_if);
  gtk_widget_show(button);

  button = gtk_button_new_with_label("Randomise");
  gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 0);
  gtk_signal_connect(GTK_OBJECT(button), "clicked",
		     GTK_SIGNAL_FUNC(chaos_harmonics_cb), syre_if);
  gtk_widget_show(button);

  frame = gtk_frame_new("Harmonics");
  gtk_box_pack_start(GTK_BOX(hbox), frame, TRUE, TRUE, 0);
  gtk_container_border_width(GTK_CONTAINER(frame), 2);
  gtk_widget_show(frame);

  hbox3 = gtk_hbox_new(FALSE, 0);
  gtk_container_add(GTK_CONTAINER(frame), hbox3);
  gtk_widget_show(hbox3);

#if 0
  syre_if->wave_curve = aube_wave_new(128, 64);
  gtk_box_pack_start(GTK_BOX(hbox), syre_if->wave_curve, TRUE, FALSE, 0);
  gtk_widget_show(syre_if->wave_curve);
#endif

  for (i = 0; i < NUM_HARMONICS; i++) {
    sprintf(nc, "%d", i);
    syre_if->sliders[i] =
      slider_int_new(nc, &(syre_if->data->overtones[i].vol), 0, 32, 1);
    gtk_signal_connect(GTK_OBJECT(SLIDER(syre_if->sliders[i])->adj), "value_changed",
		       GTK_SIGNAL_FUNC(harmonics_changed_cb), syre_if->data);
    gtk_box_pack_start(GTK_BOX(hbox3), syre_if->sliders[i], FALSE, FALSE, 0);
    gtk_widget_show(syre_if->sliders[i]);
  }

#if 0
  /*
     F R E Q  E N V E L O P E 
   */

  hbox = gtk_hbox_new(FALSE, 0);
  label = gtk_label_new("Frequency");
  gtk_notebook_append_page(GTK_NOTEBOOK(notebook), hbox, label);
  gtk_widget_show(hbox);

  frame = gtk_frame_new("Frequency Envelope");
  gtk_box_pack_start(GTK_BOX(hbox), frame, FALSE, TRUE, 0);
  gtk_container_border_width(GTK_CONTAINER(frame), 4);
  gtk_widget_show(frame);

  syre_if->freq_env_curve = gtk_gamma_curve_new();
  gtk_container_add(GTK_CONTAINER(frame), syre_if->freq_env_curve);
  gtk_curve_set_range(GTK_CURVE(GTK_GAMMA_CURVE(syre_if->freq_env_curve)->curve),
		      0, ENVELOPE_LEN, 0, 64);
  gtk_curve_set_vector(GTK_CURVE(GTK_GAMMA_CURVE(syre_if->freq_env_curve)->curve),
		       ENVELOPE_LEN, syre_if->data->freq_envelope);
  /*
     gtk_curve_set_curve_type(GTK_CURVE(GTK_GAMMA_CURVE(syre_if->freq_env_curve)->curve),
     GTK_CURVE_TYPE_LINEAR);
   */
  gtk_widget_show(syre_if->freq_env_curve);
#endif


  /*
     W A V E F O R M 
   */

  hbox = gtk_hbox_new(FALSE, 0);
  label = gtk_label_new("Waveform");
  gtk_notebook_append_page(GTK_NOTEBOOK(notebook), hbox, label);
  gtk_widget_show(hbox);

  frame = gtk_frame_new("Base wave");
  gtk_box_pack_start(GTK_BOX(hbox), frame, FALSE, FALSE, 0);
  gtk_container_border_width(GTK_CONTAINER(frame), 2);
  gtk_widget_show(frame);

  vbox = gtk_vbox_new(TRUE, 5);
  gtk_container_add(GTK_CONTAINER(frame), vbox);
  gtk_widget_show(vbox);

  button = gtk_radio_button_new_with_label(NULL, "sinwave");
  gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 0);
  gtk_signal_connect(GTK_OBJECT(button), "clicked",
		     GTK_SIGNAL_FUNC(set_sinwave_cb), syre_if->data);
  gtk_widget_show(button);

  rbgroup = gtk_radio_button_group(GTK_RADIO_BUTTON(button));

  button = gtk_radio_button_new_with_label(rbgroup, "squarewave");
  gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 0);
  gtk_signal_connect(GTK_OBJECT(button), "clicked",
		     GTK_SIGNAL_FUNC(set_squarewave_cb), syre_if->data);
  gtk_widget_show(button);
  rbgroup = gtk_radio_button_group(GTK_RADIO_BUTTON(button));

  button = gtk_radio_button_new_with_label(rbgroup, "sawtooth");
  gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 0);
  gtk_signal_connect(GTK_OBJECT(button), "clicked",
		     GTK_SIGNAL_FUNC(set_sawtooth_cb), syre_if->data);
  gtk_widget_show(button);
  rbgroup = gtk_radio_button_group(GTK_RADIO_BUTTON(button));

  button = gtk_radio_button_new_with_label(rbgroup, "whitenoise");
  gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 0);
  gtk_signal_connect(GTK_OBJECT(button), "clicked",
		     GTK_SIGNAL_FUNC(set_wnwave_cb), syre_if->data);
  gtk_widget_show(button);

  syre_if->env_tag = gtk_idle_add((GtkFunction) (get_envelopes), syre_if);

  return GTK_WIDGET(syre_if);
}

void 
syre_if_hide_cb(GtkWidget * widget, gpointer data)
{
  module *u;
  SyreIF *syre_if;

  syre_if = SYRE_IF(data);
  u = (module *) syre_if->data;
  aube_module_remove_if(u);

  gtk_idle_remove(syre_if->env_tag);
  gtk_widget_destroy(GTK_WIDGET(data));
}

void 
syre_if_close_cb(GtkWidget * widget, gpointer data)
{
  module *u;
  SyreIF *syre_if;

  syre_if = SYRE_IF(data);

  gtk_idle_remove(syre_if->env_tag);

  u = (module *) syre_if->data;
  aube_remove_module(u);

  free((SYRE_IF(data))->data);
  gtk_widget_destroy(GTK_WIDGET(data));
}

void 
syre_if_dismiss(GtkWidget * widget, gpointer data)
{
  gtk_widget_destroy(GTK_WIDGET(data));
}

void 
startstop_cb(GtkWidget * widget, gpointer data)
{
  aube_module_toggle((module *) data);
}

void 
copy_accented_cb(GtkWidget * widget, gpointer data)
{
  SyreIF *syre_if;

  syre_if = SYRE_IF(data);

  gtk_curve_set_vector(GTK_CURVE(GTK_GAMMA_CURVE(syre_if->amp_env_curve)->curve),
		       ENVELOPE_LEN, syre_if->data->amp_envelope_accented);
  gtk_curve_get_vector(GTK_CURVE(GTK_GAMMA_CURVE(SYRE_IF(data)->amp_env_curve)->curve), ENVELOPE_LEN, SYRE_IF(data)->data->amp_envelope);
  gtk_curve_set_vector(GTK_CURVE(GTK_GAMMA_CURVE(syre_if->pitch_env_curve)->curve), ENVELOPE_LEN, syre_if->data->pitch_envelope_accented);
  gtk_curve_get_vector(GTK_CURVE(GTK_GAMMA_CURVE(SYRE_IF(data)->pitch_env_curve)->curve), ENVELOPE_LEN, SYRE_IF(data)->data->pitch_envelope);
}

void 
copy_unaccented_cb(GtkWidget * widget, gpointer data)
{
  SyreIF *syre_if;

  syre_if = SYRE_IF(data);

  gtk_curve_set_vector(GTK_CURVE(GTK_GAMMA_CURVE(syre_if->amp_env_curve_accented)->curve), ENVELOPE_LEN, syre_if->data->amp_envelope);
  gtk_curve_get_vector(GTK_CURVE(GTK_GAMMA_CURVE(SYRE_IF(data)->amp_env_curve_accented)->curve), ENVELOPE_LEN, SYRE_IF(data)->data->amp_envelope_accented);
  gtk_curve_set_vector(GTK_CURVE(GTK_GAMMA_CURVE(syre_if->pitch_env_curve_accented)->curve), ENVELOPE_LEN, syre_if->data->pitch_envelope);
  gtk_curve_get_vector(GTK_CURVE(GTK_GAMMA_CURVE(SYRE_IF(data)->pitch_env_curve_accented)->curve), ENVELOPE_LEN, SYRE_IF(data)->data->pitch_envelope_accented);
}

void 
chaos_harmonics_cb(GtkWidget * widget, gpointer data)
{
  chaos_harmonics(SYRE_IF(data)->data);
  refresh_sliders(SYRE_IF(data));
}

void 
clear_harmonics_cb(GtkWidget * widget, gpointer data)
{
  clear_harmonics(SYRE_IF(data)->data);
  refresh_sliders(SYRE_IF(data));
}

void 
clear_odd_harmonics_cb(GtkWidget * widget, gpointer data)
{
  clear_odd_harmonics(SYRE_IF(data)->data);
  refresh_sliders(SYRE_IF(data));
}

void 
harmonics_changed_cb(GtkWidget * widget, gpointer data)
{
  ((syre_if_data *) data)->harmonics_changed = 1;
}

void 
refresh_sliders(SyreIF * syre_if)
{
  int i;

  for (i = 0; i < NUM_HARMONICS; i++) {
    slider_refresh(syre_if->sliders[i]);
  }
}

gint 
get_envelopes(gpointer data)
{
#if 0
  gfloat tmp_env[ENVELOPE_LEN];
  int i;
#endif

  if ((((module *) data)->on) && (((syre_if_data *) (SYRE_IF(data)->data))->env_i <= ENVELOPE_POINTS_PER_TICK)) {

    gtk_curve_get_vector(GTK_CURVE(GTK_GAMMA_CURVE(SYRE_IF(data)->amp_env_curve)->curve), ENVELOPE_LEN, SYRE_IF(data)->data->amp_envelope);
    gtk_curve_get_vector(GTK_CURVE(GTK_GAMMA_CURVE(SYRE_IF(data)->pitch_env_curve)->curve), ENVELOPE_LEN, SYRE_IF(data)->data->pitch_envelope);
    gtk_curve_get_vector(GTK_CURVE(GTK_GAMMA_CURVE(SYRE_IF(data)->amp_env_curve_accented)->curve), ENVELOPE_LEN, SYRE_IF(data)->data->amp_envelope_accented);
    gtk_curve_get_vector(GTK_CURVE(GTK_GAMMA_CURVE(SYRE_IF(data)->pitch_env_curve_accented)->curve), ENVELOPE_LEN, SYRE_IF(data)->data->pitch_envelope_accented);


#if 0
    gtk_curve_get_vector(GTK_CURVE(GTK_GAMMA_CURVE(SYRE_IF(data)->freq_env_curve)->curve), ENVELOPE_LEN, tmp_env);
    for (i = 0; i < ENVELOPE_LEN; i++) {
      if (tmp_env[i] != SYRE_IF(data)->data->freq_envelope[i]) {
	SYRE_IF(data)->data->harmonics_changed = 1;
	break;
      }
    }
    if (SYRE_IF(data)->data->harmonics_changed) {
      memcpy(SYRE_IF(data)->data->freq_envelope, tmp_env, ENVELOPE_LEN * sizeof(gfloat));
    }
#endif

#if 0
    gtk_curve_get_vector(GTK_CURVE(GTK_GAMMA_CURVE(SYRE_IF(data)->resonance_env_curve)->curve), ENVELOPE_LEN, SYRE_IF(data)->data->resonance_envelope);
    gtk_curve_get_vector(GTK_CURVE(GTK_GAMMA_CURVE(SYRE_IF(data)->cutoff_env_curve)->curve), ENVELOPE_LEN, SYRE_IF(data)->data->cutoff_envelope);
#endif
  }
  return 1;
}

gint 
set_sinwave_cb(GtkWidget * widget, gpointer * data)
{
  ((syre_if_data *) data)->current_wave = sinwave;
  ((syre_if_data *) data)->harmonics_changed = 1;
  return 1;
}

gint 
set_squarewave_cb(GtkWidget * widget, gpointer * data)
{
  ((syre_if_data *) data)->current_wave = squarewave;
  ((syre_if_data *) data)->harmonics_changed = 1;
  return 1;
}

gint 
set_sawtooth_cb(GtkWidget * widget, gpointer * data)
{
  ((syre_if_data *) data)->current_wave = sawtooth;
  ((syre_if_data *) data)->harmonics_changed = 1;
  return 1;
}

gint 
set_wnwave_cb(GtkWidget * widget, gpointer * data)
{
  ((syre_if_data *) data)->current_wave = wnwave;
  ((syre_if_data *) data)->harmonics_changed = 1;
  return 1;
}
