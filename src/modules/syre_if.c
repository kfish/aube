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

void refresh_sliders(SyreIF * syre_if);
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

GType syre_if_get_type(void)
{
	static GType b_type = 0;

	if (!b_type) {
		static const GTypeInfo b_info = {
			sizeof(SyreIFClass),
			NULL,	/* base_init */
			NULL,	/* base_finalise */
			NULL,   /* class_init */
			NULL,	/* class_finalize */
			NULL,	/* class_data */
			sizeof(SyreIF),
			0,	/* n_preallocs */
			NULL    /* init */
		};

		b_type = g_type_register_static(MODULEWINDOW_TYPE,
						"SyreIF", &b_info, 0);
	}
	return b_type;
}

static void syre_if_destroy_cb(GtkWidget * widget, gpointer data)
{
	SyreIF * ui = SYRE_IF(data);

	gtk_idle_remove(ui->env_tag);
}

GtkWidget *syre_if_new(syre_if_data * mod)
{
	SyreIF *syre_if;
	GtkWidget *vbox, *hbox, *hbox2, *hbox3;
	GtkWidget *notebook;
	GtkWidget *label;
	GtkWidget *frame;
	GtkWidget *button;
	GSList *rbgroup;

	int i;
	char nc[3];

	syre_if = gtk_type_new(syre_if_get_type());

	modulewindow_set_module (MODULEWINDOW(syre_if), (module *)mod);

	g_signal_connect(G_OBJECT(syre_if), "destroy",
			 G_CALLBACK(syre_if_destroy_cb), syre_if);

	/*
	   S Y N T H 
	 */

	hbox = MODULEWINDOW(syre_if)->headbox;

	button = inputoption_new((char *) "Sequencer:", (module *)mod, 0);
	gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 2);
	gtk_widget_show(button);

	button = outputlabel_new((module *)mod, 0);
	gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 2);
	gtk_widget_show(button);

	hbox2 = gtk_hbox_new(FALSE, 5);
	gtk_box_pack_start(GTK_BOX(MODULEWINDOW(syre_if)->mainbox), hbox2, TRUE, TRUE, 0);
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
	g_signal_connect(G_OBJECT(button), "clicked",
			 G_CALLBACK(copy_accented_cb), syre_if);
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
	gtk_curve_set_vector(GTK_CURVE (GTK_GAMMA_CURVE(syre_if->amp_env_curve)->curve),
                             ENVELOPE_LEN, mod->amp_envelope);
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
                             ENVELOPE_LEN, mod->pitch_envelope);
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
	g_signal_connect(G_OBJECT(button), "clicked",
			 G_CALLBACK(copy_unaccented_cb), syre_if);
	gtk_widget_show(button);

	/*
	   AMPLITUDE ENVELOPE 
	 */

	frame = gtk_frame_new("Amplitude Envelope");
	gtk_box_pack_start(GTK_BOX(hbox), frame, FALSE, FALSE, 0);
	gtk_container_border_width(GTK_CONTAINER(frame), 4);
	gtk_widget_show(frame);

	syre_if->amp_env_curve_accented = gtk_gamma_curve_new();
	gtk_container_add(GTK_CONTAINER(frame),
			  syre_if->amp_env_curve_accented);
	gtk_curve_set_range(GTK_CURVE(GTK_GAMMA_CURVE(syre_if->amp_env_curve_accented)->curve), 0,
			    ENVELOPE_LEN, 0, 64);
	gtk_curve_set_vector(GTK_CURVE(GTK_GAMMA_CURVE(syre_if->amp_env_curve_accented)->curve),
			     ENVELOPE_LEN, mod->amp_envelope_accented);
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
	gtk_container_add(GTK_CONTAINER(frame),
			  syre_if->pitch_env_curve_accented);
	gtk_curve_set_range(GTK_CURVE
			    (GTK_GAMMA_CURVE
			     (syre_if->pitch_env_curve_accented)->curve),
			    0, ENVELOPE_LEN, 0, 64);
	gtk_curve_set_vector(GTK_CURVE(GTK_GAMMA_CURVE(syre_if->pitch_env_curve_accented)->curve),
			     ENVELOPE_LEN, mod->pitch_envelope_accented);
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
	g_signal_connect(G_OBJECT(button), "clicked",
			 G_CALLBACK(clear_harmonics_cb), syre_if);
	gtk_widget_show(button);

	button = gtk_button_new_with_label("Clear Odds");
	gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 0);
	g_signal_connect(G_OBJECT(button), "clicked",
			 G_CALLBACK(clear_odd_harmonics_cb), syre_if);
	gtk_widget_show(button);

	button = gtk_button_new_with_label("Randomise");
	gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 0);
	g_signal_connect(G_OBJECT(button), "clicked",
			 G_CALLBACK(chaos_harmonics_cb), syre_if);
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
	gtk_box_pack_start(GTK_BOX(hbox), syre_if->wave_curve, TRUE, FALSE,
			   0);
	gtk_widget_show(syre_if->wave_curve);
#endif

	for (i = 0; i < NUM_HARMONICS; i++) {
		snprintf(nc, sizeof(nc), "%d", i);
		syre_if->sliders[i] = slider_int_new(nc, &mod->overtones[i].vol, 0, 32, 1);
		g_signal_connect(G_OBJECT
				 (SLIDER(syre_if->sliders[i])->adj),
				 "value_changed",
				 G_CALLBACK(harmonics_changed_cb),
				 mod);
		gtk_box_pack_start(GTK_BOX(hbox3), syre_if->sliders[i],
				   FALSE, FALSE, 0);
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
                             ENVELOPE_LEN, mod->freq_envelope);
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
	g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(set_sinwave_cb), mod);
	gtk_widget_show(button);

	rbgroup = gtk_radio_button_group(GTK_RADIO_BUTTON(button));

	button = gtk_radio_button_new_with_label(rbgroup, "squarewave");
	gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 0);
	g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(set_squarewave_cb), mod);
	gtk_widget_show(button);
	rbgroup = gtk_radio_button_group(GTK_RADIO_BUTTON(button));

	button = gtk_radio_button_new_with_label(rbgroup, "sawtooth");
	gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 0);
	g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(set_sawtooth_cb), mod);
	gtk_widget_show(button);
	rbgroup = gtk_radio_button_group(GTK_RADIO_BUTTON(button));

	button = gtk_radio_button_new_with_label(rbgroup, "whitenoise");
	gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 0);
	g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(set_wnwave_cb), mod);
	gtk_widget_show(button);

	syre_if->env_tag = gtk_idle_add((GtkFunction) (get_envelopes), syre_if);

	return GTK_WIDGET(syre_if);
}

void copy_accented_cb(GtkWidget * widget, gpointer data)
{
	SyreIF *syre_if = SYRE_IF(data);
        syre_if_data * mod = (syre_if_data *)MODULEWINDOW(syre_if)->module;

	gtk_curve_set_vector(GTK_CURVE(GTK_GAMMA_CURVE(syre_if->amp_env_curve)->curve),
                             ENVELOPE_LEN, mod->amp_envelope_accented);
	gtk_curve_get_vector(GTK_CURVE(GTK_GAMMA_CURVE(SYRE_IF(data)->amp_env_curve)->curve),
			     ENVELOPE_LEN, mod->amp_envelope);
	gtk_curve_set_vector(GTK_CURVE(GTK_GAMMA_CURVE(syre_if->pitch_env_curve)->curve),
                             ENVELOPE_LEN, mod->pitch_envelope_accented);
	gtk_curve_get_vector(GTK_CURVE(GTK_GAMMA_CURVE(SYRE_IF(data)->pitch_env_curve)->curve),
			     ENVELOPE_LEN, mod->pitch_envelope);
}

void copy_unaccented_cb(GtkWidget * widget, gpointer data)
{
	SyreIF *syre_if = SYRE_IF(data);
        syre_if_data * mod = (syre_if_data *)MODULEWINDOW(syre_if)->module;

	gtk_curve_set_vector(GTK_CURVE(GTK_GAMMA_CURVE(syre_if->amp_env_curve_accented)->curve),
			     ENVELOPE_LEN, mod->amp_envelope);
	gtk_curve_get_vector(GTK_CURVE(GTK_GAMMA_CURVE(SYRE_IF(data)->amp_env_curve_accented)->curve),
                             ENVELOPE_LEN, mod->amp_envelope_accented);
	gtk_curve_set_vector(GTK_CURVE(GTK_GAMMA_CURVE(syre_if->pitch_env_curve_accented)->curve),
			     ENVELOPE_LEN, mod->pitch_envelope);
	gtk_curve_get_vector(GTK_CURVE(GTK_GAMMA_CURVE(SYRE_IF(data)->pitch_env_curve_accented)->curve),
                             ENVELOPE_LEN, mod->pitch_envelope_accented);
}

void chaos_harmonics_cb(GtkWidget * widget, gpointer data)
{
	SyreIF *syre_if = SYRE_IF(data);
        syre_if_data * mod = (syre_if_data *)MODULEWINDOW(syre_if)->module;

	chaos_harmonics(mod);
	refresh_sliders(syre_if);
}

void clear_harmonics_cb(GtkWidget * widget, gpointer data)
{
	SyreIF *syre_if = SYRE_IF(data);
        syre_if_data * mod = (syre_if_data *)MODULEWINDOW(syre_if)->module;

	clear_harmonics(mod);
	refresh_sliders(syre_if);
}

void clear_odd_harmonics_cb(GtkWidget * widget, gpointer data)
{
	SyreIF *syre_if = SYRE_IF(data);
        syre_if_data * mod = (syre_if_data *)MODULEWINDOW(syre_if)->module;

	clear_odd_harmonics(mod);
	refresh_sliders(syre_if);
}

void harmonics_changed_cb(GtkWidget * widget, gpointer data)
{
	((syre_if_data *) data)->harmonics_changed = 1;
}

void refresh_sliders(SyreIF * syre_if)
{
	int i;

	for (i = 0; i < NUM_HARMONICS; i++) {
		slider_refresh(syre_if->sliders[i]);
	}
}

gint get_envelopes(gpointer data)
{
	SyreIF *syre_if = SYRE_IF(data);
        syre_if_data * mod = (syre_if_data *)MODULEWINDOW(syre_if)->module;

#if 0
	gfloat tmp_env[ENVELOPE_LEN];
	int i;
#endif

	if ( ((module *)mod)->on && mod->env_i <= ENVELOPE_POINTS_PER_TICK) {

		gtk_curve_get_vector(GTK_CURVE(GTK_GAMMA_CURVE(SYRE_IF(data)->amp_env_curve)->curve),
                                     ENVELOPE_LEN, mod->amp_envelope);
		gtk_curve_get_vector(GTK_CURVE(GTK_GAMMA_CURVE(SYRE_IF(data)->pitch_env_curve)->curve),
                                     ENVELOPE_LEN, mod->pitch_envelope);
		gtk_curve_get_vector(GTK_CURVE(GTK_GAMMA_CURVE(SYRE_IF(data)->amp_env_curve_accented)->curve),
				     ENVELOPE_LEN, mod->amp_envelope_accented);
		gtk_curve_get_vector(GTK_CURVE(GTK_GAMMA_CURVE(SYRE_IF(data)->pitch_env_curve_accented)->curve),
				     ENVELOPE_LEN, mod->pitch_envelope_accented);


#if 0
		gtk_curve_get_vector(GTK_CURVE(GTK_GAMMA_CURVE(SYRE_IF(data)->freq_env_curve)->curve),
                                     ENVELOPE_LEN, tmp_env);
		for (i = 0; i < ENVELOPE_LEN; i++) {
			if (tmp_env[i] != mod->freq_envelope[i]) {
				mod->harmonics_changed = 1;
				break;
			}
		}
		if (mod->harmonics_changed) {
			memcpy(mod->freq_envelope, tmp_env, ENVELOPE_LEN * sizeof(gfloat));
		}
#endif

#if 0
		gtk_curve_get_vector(GTK_CURVE(GTK_GAMMA_CURVE(SYRE_IF(data)->resonance_env_curve)->curve),
				     ENVELOPE_LEN, mod->resonance_envelope);
		gtk_curve_get_vector(GTK_CURVE(GTK_GAMMA_CURVE(SYRE_IF(data)->cutoff_env_curve)->curve),
                                     ENVELOPE_LEN, mod->cutoff_envelope);
#endif
	}
	return 1;
}

gint set_sinwave_cb(GtkWidget * widget, gpointer * data)
{
	((syre_if_data *) data)->current_wave = sinwave;
	((syre_if_data *) data)->harmonics_changed = 1;
	return 1;
}

gint set_squarewave_cb(GtkWidget * widget, gpointer * data)
{
	((syre_if_data *) data)->current_wave = squarewave;
	((syre_if_data *) data)->harmonics_changed = 1;
	return 1;
}

gint set_sawtooth_cb(GtkWidget * widget, gpointer * data)
{
	((syre_if_data *) data)->current_wave = sawtooth;
	((syre_if_data *) data)->harmonics_changed = 1;
	return 1;
}

gint set_wnwave_cb(GtkWidget * widget, gpointer * data)
{
	((syre_if_data *) data)->current_wave = wnwave;
	((syre_if_data *) data)->harmonics_changed = 1;
	return 1;
}
