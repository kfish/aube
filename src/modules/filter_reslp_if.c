/*
 * AUBE/Metadecks Live
 *
 * Copyright (C) 1998-2009 Conrad Parker
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

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
GtkWidget *filter_reslp_if_new(filter_reslp * mod);

void filter_reslp_usetoggle_cb(GtkWidget * widget, gpointer data);
gint filter_reslp_get_envelopes(gpointer data);

GType filter_reslp_if_get_type(void)
{
	static GType b_type = 0;

	if (!b_type) {
		static const GTypeInfo b_info = {
			sizeof(FilterResLP_IFClass),
			NULL,	/* base_init */
			NULL,	/* base_finalise */
			(GClassInitFunc) filter_reslp_if_class_init,
			NULL,	/* class_finalize */
			NULL,	/* class_data */
			sizeof(FilterResLP_IF),
			0,	/* n_preallocs */
			(GInstanceInitFunc) filter_reslp_if_init,
		};

		b_type = g_type_register_static(MODULEWINDOW_TYPE,
						"FilterResLP_IF",
						&b_info, 0);
	}
	return b_type;
}

static void filter_reslp_if_class_init(FilterResLP_IFClass * klass)
{

}

static void filter_reslp_if_init(FilterResLP_IF * filter_reslp_if)
{
}

static void filter_reslp_if_destroy_cb(GtkWidget * widget, gpointer data)
{
	FilterResLP_IF * ui = FILTERRESLP_IF(data);

	gtk_idle_remove(ui->env_tag);
}

GtkWidget *filter_reslp_if_new(filter_reslp * mod)
{
	FilterResLP_IF *filter_reslp_if;
	GtkWidget *hbox, *hbox2, *hbox3;
	GtkWidget *frame;
	GtkWidget *button;
	GtkWidget *slider;

	filter_reslp_if = gtk_type_new(filter_reslp_if_get_type());

	modulewindow_set_module (MODULEWINDOW(filter_reslp_if), (module *)mod);

        hbox = MODULEWINDOW(filter_reslp_if)->headbox;

	button = inputoption_new((char *) "Input:", (module *)mod, 0);
	gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, TRUE, 2);
	gtk_widget_show(button);

	button =
	    inputoption_new((char *) "Trigger:", (module *)mod, 1);
	gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, TRUE, 2);
	gtk_widget_show(button);

	button = gtk_check_button_new_with_label("Use trigger");
	gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 2);
	gtk_toggle_button_set_state(GTK_TOGGLE_BUTTON(button),
				    mod->use_trigger);
	g_signal_connect(G_OBJECT(button), "clicked",
			 G_CALLBACK(filter_reslp_usetoggle_cb),
			 mod);
	gtk_widget_show(button);

	button = outputlabel_new((module *)mod, 0);
	gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 2);
	gtk_widget_show(button);

	hbox2 = gtk_hbox_new(FALSE, 5);
	gtk_box_pack_start(GTK_BOX(MODULEWINDOW(filter_reslp_if)->mainbox), hbox2, TRUE, TRUE, 0);
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

	slider = slider_int_new("Vol", &mod->vol, 0, 64, 1);
	gtk_box_pack_start(GTK_BOX(hbox3), slider, TRUE, FALSE, 0);
	gtk_widget_show(slider);

#if 0
	slider = slider_int_new("Tune", &mod->tune, 1, 1024, 1);
	gtk_box_pack_start(GTK_BOX(hbox3), slider, TRUE, FALSE, 0);
	g_signal_connect(G_OBJECT(SLIDER(slider)->adj), "value_changed",
			 G_CALLBACK(harmonics_changed_cb), mod);
	gtk_widget_show(slider);
#endif

	slider = slider_int_new("Pan", &mod->pan, 0, 32, 0);
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

	slider =
	    slider_int_new("Res.", &mod->resonance_base, 0, 200, 1);
	gtk_box_pack_start(GTK_BOX(hbox3), slider, FALSE, TRUE, 0);
	gtk_widget_show(slider);

	filter_reslp_if->resonance_env_curve = gtk_gamma_curve_new();
	gtk_box_pack_start(GTK_BOX(hbox3),
			   filter_reslp_if->resonance_env_curve, TRUE,
			   TRUE, 0);
	gtk_curve_set_range(GTK_CURVE
			    (GTK_GAMMA_CURVE
			     (filter_reslp_if->resonance_env_curve)->
			     curve), 0, ENVELOPE_LEN, 0, 100);
	gtk_curve_set_vector(GTK_CURVE
			     (GTK_GAMMA_CURVE
			      (filter_reslp_if->resonance_env_curve)->
			      curve), ENVELOPE_LEN,
			     mod->resonance_envelope);
	gtk_widget_show(filter_reslp_if->resonance_env_curve);

	frame = gtk_frame_new("Cutoff");
	gtk_box_pack_start(GTK_BOX(hbox2), frame, TRUE, TRUE, 0);
	gtk_container_border_width(GTK_CONTAINER(frame), 4);
	gtk_widget_show(frame);

	hbox3 = gtk_hbox_new(FALSE, 0);
	gtk_container_add(GTK_CONTAINER(frame), hbox3);
	gtk_widget_show(hbox3);

	slider = slider_int_new("Cutoff", &mod->cutoff_base, 20, 20000, 1);
	gtk_box_pack_start(GTK_BOX(hbox3), slider, FALSE, TRUE, 0);
	gtk_widget_show(slider);

	filter_reslp_if->cutoff_env_curve = gtk_gamma_curve_new();
	gtk_box_pack_start(GTK_BOX(hbox3),
			   filter_reslp_if->cutoff_env_curve, TRUE, TRUE,
			   0);
	gtk_curve_set_range(GTK_CURVE
			    (GTK_GAMMA_CURVE
			     (filter_reslp_if->cutoff_env_curve)->curve),
			    0, ENVELOPE_LEN, 0, 100);
	gtk_curve_set_vector(GTK_CURVE
			     (GTK_GAMMA_CURVE
			      (filter_reslp_if->cutoff_env_curve)->curve),
			     ENVELOPE_LEN,
			     mod->cutoff_envelope);
	gtk_widget_show(filter_reslp_if->cutoff_env_curve);


	filter_reslp_if->env_tag =
	    gtk_idle_add((GtkFunction) (filter_reslp_get_envelopes),
			 filter_reslp_if);

        /* Remove the idle when destroying the window */
	g_signal_connect(G_OBJECT(filter_reslp_if), "destroy",
			 G_CALLBACK(filter_reslp_if_destroy_cb),
			 filter_reslp_if);

	return GTK_WIDGET(filter_reslp_if);
}

void filter_reslp_usetoggle_cb(GtkWidget * widget, gpointer data)
{
	((filter_reslp *) data)->use_trigger =
	    1 - ((filter_reslp *) data)->use_trigger;
}

gint filter_reslp_get_envelopes(gpointer data)
{
	FilterResLP_IF * ui = FILTERRESLP_IF(data);
        filter_reslp * mod = (filter_reslp *)MODULEWINDOW(ui)->module;

	if (((module *)data)->on && (mod->env_i < ENVELOPE_POINTS_PER_TICK)) {
		gtk_curve_get_vector(GTK_CURVE
				     (GTK_GAMMA_CURVE
				      (FILTERRESLP_IF(data)->resonance_env_curve)->curve),
				     ENVELOPE_LEN,
				     mod->resonance_envelope);
		gtk_curve_get_vector(GTK_CURVE
				     (GTK_GAMMA_CURVE
				      (FILTERRESLP_IF(data)->cutoff_env_curve)->curve),
				     ENVELOPE_LEN,
				     mod->cutoff_envelope);
	}
	return 1;
}
