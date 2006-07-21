#include <stdlib.h>
#include <strings.h>
#include <gtk/gtk.h>
#include "aube.h"
#include "master.h"
#include "filter_reslp_if.h"
#include "slider.h"
#include "inputoption.h"
#include "outputlabel.h"

extern int tick;

static void filter_reslp_if_class_init(FilterResLP_IFClass * klass);
static void filter_reslp_if_init(FilterResLP_IF * b);
GtkWidget *filter_reslp_if_new(filter_reslp * fr);
void filter_reslp_if_dismiss(GtkWidget * widget, gpointer data);

void filter_reslp_if_close_cb(GtkWidget * widget, gpointer data);
void filter_reslp_if_hide_cb(GtkWidget * widget, gpointer data);
void filter_reslp_startstop_cb(GtkWidget * widget, gpointer data);
void filter_reslp_usetoggle_cb(GtkWidget * widget, gpointer data);
gint filter_reslp_get_envelopes(gpointer data);

GType
filter_reslp_if_get_type(void)
{
  static GType b_type = 0;

  if (!b_type) {
    static const GTypeInfo b_info =
    {
      sizeof(FilterResLP_IFClass),
      NULL, /* base_init */
	  NULL, /* base_finalise */
      (GClassInitFunc) filter_reslp_if_class_init,
	  NULL, /* class_finalize */
	  NULL, /* class_data */
      sizeof(FilterResLP_IF),
	  0, /* n_preallocs */
	  (GInstanceInitFunc) filter_reslp_if_init,
    };

    b_type = g_type_register_static(GTK_TYPE_WINDOW,
                                                      "FilterResLP_IF",
	                                                   &b_info, 0);
  }
  return b_type;
}

static void
filter_reslp_if_class_init(FilterResLP_IFClass * klass)
{

}

static void
filter_reslp_if_init(FilterResLP_IF * filter_reslp_if)
{
}

GtkWidget *
filter_reslp_if_new(filter_reslp * fr)
{
  FilterResLP_IF *filter_reslp_if;
  GtkWidget *vbox2, *hbox, *hbox2, *hbox3;
  GtkWidget *frame;
  GtkWidget *button;
  GtkWidget *slider;

  filter_reslp_if = gtk_type_new(filter_reslp_if_get_type());

  filter_reslp_if->data = fr;

  gtk_window_set_title(GTK_WINDOW(filter_reslp_if), ((module *) filter_reslp_if->data)->u_label);

  gtk_container_border_width(GTK_CONTAINER(filter_reslp_if), 1);

#if 0
  /*
     when the window is given the "delete_event" signal - this is
     * given by the window manager - usually the close option or on the
     * titlebar - we ask it to call the delete_event() function
     * as defined above. The data passed to the callback function is
     * NULL and is ignored in the callback. 
   */
  g_signal_connect(G_OBJECT(filter_reslp_if), "delete_event",
		     G_CALLBACK(delete_event), NULL);
#endif

#if 1
  /*
     here we connect the "destroy" event to a signal handler.
     * This event occurs when we call gtk_widget_destroy() on the
     * window, or if we return "TRUE" in the "delete_event" callback. 
   */
  g_signal_connect(G_OBJECT(filter_reslp_if), "destroy",
		     G_CALLBACK(filter_reslp_if_close_cb), filter_reslp_if);
#endif

  vbox2 = gtk_vbox_new(FALSE, 5);
  gtk_container_add(GTK_CONTAINER(filter_reslp_if), vbox2);
  gtk_widget_show(vbox2);

  /*
     I N P U T 
   */

  hbox = gtk_hbox_new(FALSE, 0);
  gtk_box_pack_start(GTK_BOX(vbox2), hbox, FALSE, TRUE, 0);
  gtk_widget_show(hbox);

  button = gtk_toggle_button_new_with_label("On");
  gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 1);
  gtk_toggle_button_set_state(GTK_TOGGLE_BUTTON(button),
			      filter_reslp_if->data->module.on);
  g_signal_connect(G_OBJECT(button), "clicked",
	 G_CALLBACK(filter_reslp_startstop_cb), filter_reslp_if->data);
  gtk_widget_show(button);

  button = opsmenu_new((module *) filter_reslp_if->data,
		       GTK_WIDGET(filter_reslp_if), filter_reslp_if_hide_cb,
		       filter_reslp_if_close_cb);
  gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 4);
  gtk_widget_show(button);

  button = inputoption_new((char *) "Input:",
		       (module *) FILTERRESLP_IF(filter_reslp_if)->data, 0);
  gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, TRUE, 2);
  gtk_widget_show(button);

  button = inputoption_new((char *) "Trigger:", (module *) FILTERRESLP_IF(filter_reslp_if)->data, 1);
  gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, TRUE, 2);
  gtk_widget_show(button);

  button = gtk_check_button_new_with_label("Use trigger");
  gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 2);
  gtk_toggle_button_set_state(GTK_TOGGLE_BUTTON(button),
			      filter_reslp_if->data->use_trigger);
  g_signal_connect(G_OBJECT(button), "clicked",
	 G_CALLBACK(filter_reslp_usetoggle_cb), filter_reslp_if->data);
  gtk_widget_show(button);

  button = outputlabel_new((module *) FILTERRESLP_IF(filter_reslp_if)->data, 0);
  gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 2);
  gtk_widget_show(button);

  button = gtk_check_button_new_with_label("Use trigger");
  hbox2 = gtk_hbox_new(FALSE, 5);
  gtk_box_pack_start(GTK_BOX(vbox2), hbox2, TRUE, TRUE, 0);
  gtk_widget_show(hbox2);


#if 0
  frame = gtk_frame_new("FilterResLP");
  gtk_box_pack_start(GTK_BOX(hbox2), frame, FALSE, TRUE, 0);
/*
   gtk_container_border_width(GTK_CONTAINER(frame), 4);
 */
  gtk_widget_show(frame);

  hbox = gtk_hbox_new(FALSE, 0);
  gtk_container_add(GTK_CONTAINER(frame), hbox);
  gtk_widget_show(hbox);

  vbox = gtk_vbox_new(FALSE, 0);
  gtk_box_pack_start(GTK_BOX(hbox), vbox, TRUE, FALSE, 0);
  gtk_widget_show(vbox);

  hbox3 = gtk_hbox_new(FALSE, 0);
  gtk_box_pack_start(GTK_BOX(vbox), hbox3, TRUE, TRUE, 0);
  gtk_widget_show(hbox3);

  slider = slider_int_new("Vol", &(filter_reslp_if->data->vol), 0, 64, 1);
  gtk_box_pack_start(GTK_BOX(hbox3), slider, TRUE, FALSE, 0);
  gtk_widget_show(slider);

#if 0
  slider = slider_int_new("Tune", &(filter_reslp_if->data->tune), 1, 1024, 1);
  gtk_box_pack_start(GTK_BOX(hbox3), slider, TRUE, FALSE, 0);
  g_signal_connect(G_OBJECT(SLIDER(slider)->adj), "value_changed",
	      G_CALLBACK(harmonics_changed_cb), filter_reslp_if->data);
  gtk_widget_show(slider);
#endif

  slider = slider_int_new("Pan", &(filter_reslp_if->data->pan), 0, 32, 0);
  gtk_box_pack_start(GTK_BOX(vbox), slider, FALSE, FALSE, 0);
  gtk_widget_show(slider);
#endif


  /*
     R E S O N A N C E 
   */

  frame = gtk_frame_new("Resonance");
  gtk_box_pack_start(GTK_BOX(hbox2), frame, TRUE, TRUE, 0);
  gtk_container_border_width(GTK_CONTAINER(frame), 4);
  gtk_widget_show(frame);

  hbox3 = gtk_hbox_new(FALSE, 0);
  gtk_container_add(GTK_CONTAINER(frame), hbox3);
  gtk_widget_show(hbox3);

  slider = slider_int_new("Res.", &(filter_reslp_if->data->resonance_base), 0, 200, 1);
  gtk_box_pack_start(GTK_BOX(hbox3), slider, FALSE, TRUE, 0);
  gtk_widget_show(slider);

  filter_reslp_if->resonance_env_curve = gtk_gamma_curve_new();
  gtk_box_pack_start(GTK_BOX(hbox3), filter_reslp_if->resonance_env_curve, TRUE, TRUE, 0);
  gtk_curve_set_range(GTK_CURVE(GTK_GAMMA_CURVE(filter_reslp_if->resonance_env_curve)->curve),
		      0, ENVELOPE_LEN, 0, 100);
  gtk_curve_set_vector(GTK_CURVE(GTK_GAMMA_CURVE(filter_reslp_if->resonance_env_curve)->curve),
		   ENVELOPE_LEN, filter_reslp_if->data->resonance_envelope);
  gtk_widget_show(filter_reslp_if->resonance_env_curve);

  frame = gtk_frame_new("Cutoff");
  gtk_box_pack_start(GTK_BOX(hbox2), frame, TRUE, TRUE, 0);
  gtk_container_border_width(GTK_CONTAINER(frame), 4);
  gtk_widget_show(frame);

  hbox3 = gtk_hbox_new(FALSE, 0);
  gtk_container_add(GTK_CONTAINER(frame), hbox3);
  gtk_widget_show(hbox3);

  slider = slider_int_new("Cutoff", &(filter_reslp_if->data->cutoff_base),
		      20, 20000, 1);
  gtk_box_pack_start(GTK_BOX(hbox3), slider, FALSE, TRUE, 0);
  gtk_widget_show(slider);

  filter_reslp_if->cutoff_env_curve = gtk_gamma_curve_new();
  gtk_box_pack_start(GTK_BOX(hbox3), filter_reslp_if->cutoff_env_curve, TRUE, TRUE, 0);
  gtk_curve_set_range(GTK_CURVE(GTK_GAMMA_CURVE(filter_reslp_if->cutoff_env_curve)->curve),
		      0, ENVELOPE_LEN, 0, 100);
  gtk_curve_set_vector(GTK_CURVE(GTK_GAMMA_CURVE(filter_reslp_if->cutoff_env_curve)->curve),
		       ENVELOPE_LEN, filter_reslp_if->data->cutoff_envelope);
  gtk_widget_show(filter_reslp_if->cutoff_env_curve);


  filter_reslp_if->env_tag = gtk_idle_add((GtkFunction) (filter_reslp_get_envelopes), filter_reslp_if);

  return GTK_WIDGET(filter_reslp_if);
}

void 
filter_reslp_if_hide_cb(GtkWidget * widget, gpointer data)
{
  module *u;
  FilterResLP_IF *filter_reslp_if;

  filter_reslp_if = FILTERRESLP_IF(data);

  u = (module *) filter_reslp_if->data;
  aube_module_remove_if(u);

  gtk_idle_remove(filter_reslp_if->env_tag);
  gtk_widget_destroy(GTK_WIDGET(data));
}

void 
filter_reslp_if_close_cb(GtkWidget * widget, gpointer data)
{
  module *u;
  FilterResLP_IF *filter_reslp_if;

  filter_reslp_if = FILTERRESLP_IF(data);

  gtk_idle_remove(filter_reslp_if->env_tag);

  u = (module *) filter_reslp_if->data;
  aube_remove_module(u);

  free((FILTERRESLP_IF(data))->data);
  gtk_widget_destroy(GTK_WIDGET(data));
}

void 
filter_reslp_if_dismiss(GtkWidget * widget, gpointer data)
{
  gtk_widget_destroy(GTK_WIDGET(data));
}

void 
filter_reslp_startstop_cb(GtkWidget * widget, gpointer data)
{
  aube_module_toggle((module *) data);
}

void 
filter_reslp_usetoggle_cb(GtkWidget * widget, gpointer data)
{
  ((filter_reslp *) data)->use_trigger = 1 - ((filter_reslp *) data)->use_trigger;
}

gint 
filter_reslp_get_envelopes(gpointer data)
{
  if ((((module *) data)->on) && (((filter_reslp *) (FILTERRESLP_IF(data)->data))->env_i < ENVELOPE_POINTS_PER_TICK)) {

    gtk_curve_get_vector(GTK_CURVE(GTK_GAMMA_CURVE(FILTERRESLP_IF(data)->resonance_env_curve)->curve), ENVELOPE_LEN, FILTERRESLP_IF(data)->data->resonance_envelope);
    gtk_curve_get_vector(GTK_CURVE(GTK_GAMMA_CURVE(FILTERRESLP_IF(data)->cutoff_env_curve)->curve), ENVELOPE_LEN, FILTERRESLP_IF(data)->data->cutoff_envelope);
  }
  return 1;
}
