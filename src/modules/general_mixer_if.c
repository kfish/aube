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

#include "general_mixer_if.h"
#include "aube.h"
#include "master.h"
#include "slider.h"
#include "inputoption.h"
#include "outputlabel.h"

static void general_mixer_if_class_init(GeneralMixerIFClass * klass);
static void general_mixer_if_init(GeneralMixerIF * b);
GtkWidget *general_mixer_if_new(general_mixer * mod);
void general_mixer_if_hide_cb(GtkWidget * widget, gpointer data);
void general_mixer_if_close_cb(GtkWidget * widget, gpointer data);
void general_mixer_if_add_input_cb(GtkWidget * widget, gpointer data);
void general_mixer_if_remove_input_cb(GtkWidget * widget, gpointer data);
void general_mixer_if_add_input(GeneralMixerIF * general_mixer_if, int i);
void general_mixer_if_remove_input(GeneralMixerIF * general_mixer_if,
				   int i);
void general_mixer_if_mute_cb(GtkWidget * widget, gpointer data);

GType general_mixer_if_get_type(void)
{
	static GType b_type = 0;

	if (!b_type) {
		static const GTypeInfo b_info = {
			sizeof(GeneralMixerIFClass),
			NULL,	/* base_init */
			NULL,	/* base_finalise */
			(GClassInitFunc) general_mixer_if_class_init,
			NULL,	/* class_finalize */
			NULL,	/* class_data */
			sizeof(GeneralMixerIF),
			0,	/* n_preallocs */
			(GInstanceInitFunc) general_mixer_if_init,
		};

		b_type = g_type_register_static(MODULEWINDOW_TYPE,
						"GeneralMixerIF",
						&b_info, 0);
	}
	return b_type;
}

static void general_mixer_if_class_init(GeneralMixerIFClass * klass)
{

}

static void general_mixer_if_init(GeneralMixerIF * general_mixer_if)
{
}

GtkWidget *general_mixer_if_new(general_mixer * mod)
{
	GeneralMixerIF *general_mixer_if;
	GtkWidget *vbox, *hbox;
	GtkWidget *button, *slider;
	gint i;

	general_mixer_if = gtk_type_new(general_mixer_if_get_type());

	modulewindow_set_module (MODULEWINDOW(general_mixer_if), (module *)mod);

	general_mixer_if->data = mod;

#if 0
	g_signal_connect(G_OBJECT(general_mixer_if), "delete_event",
			 G_CALLBACK(delete_event), NULL);
#endif

#if 1
	g_signal_connect(G_OBJECT(general_mixer_if), "destroy",
			 G_CALLBACK(general_mixer_if_close_cb),
			 general_mixer_if);
#endif

	hbox = MODULEWINDOW(general_mixer_if)->headbox;

	button =
	    opsmenu_new((module *) general_mixer_if->data,
			GTK_WIDGET(general_mixer_if),
			general_mixer_if_hide_cb,
			general_mixer_if_close_cb);
	gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 1);
	gtk_widget_show(button);

	button = gtk_button_new_with_label("Add");
	gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 2);
	g_signal_connect(G_OBJECT(button), "clicked",
			 G_CALLBACK(general_mixer_if_add_input_cb),
			 general_mixer_if);
	gtk_widget_show(button);

	button = gtk_button_new_with_label("Remove");
	gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 2);
	g_signal_connect(G_OBJECT(button), "clicked",
			 G_CALLBACK(general_mixer_if_remove_input_cb),
			 general_mixer_if);
	gtk_widget_show(button);

	button =
	    outputlabel_new((module *) GENERAL_MIXER_IF(general_mixer_if)->
			    data, 0);
	gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 2);
	gtk_widget_show(button);

	general_mixer_if->hbox = gtk_hbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(MODULEWINDOW(general_mixer_if)->mainbox),
                           general_mixer_if->hbox, TRUE, TRUE, 0);
	gtk_widget_show(general_mixer_if->hbox);

	vbox = gtk_vbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(general_mixer_if->hbox), vbox, TRUE,
			   FALSE, 0);
	gtk_widget_show(vbox);

	slider =
	    slider_int_new("Master", &(general_mixer_if->data->master_vol),
			   0, 64, 1);
	gtk_box_pack_start(GTK_BOX(vbox), slider, TRUE, TRUE, 0);
	gtk_widget_show(slider);

	button = gtk_toggle_button_new_with_label("Mute");
	gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 2);
	gtk_toggle_button_set_state(GTK_TOGGLE_BUTTON(button),
				    general_mixer_if->data->master_mute);
	g_signal_connect(G_OBJECT(button), "clicked",
			 G_CALLBACK(general_mixer_if_mute_cb),
			 &(general_mixer_if->data->master_mute));
	gtk_widget_show(button);

	button = gtk_vseparator_new();
	gtk_box_pack_start(GTK_BOX(general_mixer_if->hbox), button, TRUE,
			   FALSE, 2);
	gtk_widget_show(button);

	for (i = 0; i < general_mixer_if->data->nr_active_channels; i++) {
		general_mixer_if_add_input(general_mixer_if, i);
	}

	return GTK_WIDGET(general_mixer_if);
}

void general_mixer_if_hide_cb(GtkWidget * widget, gpointer data)
{
	module *u;
	GeneralMixerIF *general_mixer_if;

	general_mixer_if = GENERAL_MIXER_IF(data);
	u = (module *) general_mixer_if->data;
	aube_module_remove_if(u);

	gtk_widget_destroy(GTK_WIDGET(data));
}

void general_mixer_if_close_cb(GtkWidget * widget, gpointer data)
{
	module *u;
	GeneralMixerIF *general_mixer_if;

	general_mixer_if = GENERAL_MIXER_IF(data);
	u = (module *) general_mixer_if->data;
	aube_remove_module(u);

	free((GENERAL_MIXER_IF(data))->data);
	gtk_widget_destroy(GTK_WIDGET(data));
}

void general_mixer_if_add_input_cb(GtkWidget * widget, gpointer data)
{
	GeneralMixerIF *general_mixer_if;
	int i;

	general_mixer_if = GENERAL_MIXER_IF(data);
	if ((i = general_mixer_add_input(general_mixer_if->data)) > 0) {
		general_mixer_if_add_input(general_mixer_if, i);
	}
}

void general_mixer_if_remove_input_cb(GtkWidget * widget, gpointer data)
{
	GeneralMixerIF *general_mixer_if;
	int i;

	general_mixer_if = GENERAL_MIXER_IF(data);
	if ((i = general_mixer_remove_input(general_mixer_if->data)) > 0) {
		general_mixer_if_remove_input(general_mixer_if, i);
	}
}

void general_mixer_if_mute_cb(GtkWidget * widget, gpointer data)
{
	*((int *) data) = 1 - *((int *) data);
}

void general_mixer_if_add_input(GeneralMixerIF * general_mixer_if, int i)
{
	char buf[8];
	GtkWidget *vbox, *button, *slider;

	vbox = gtk_vbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(general_mixer_if->hbox), vbox, TRUE,
			   TRUE, 0);
	gtk_widget_show(vbox);

	snprintf(buf, sizeof(buf), "%d:", i);
	button = inputoption_new(buf,
				 (module *)
				 GENERAL_MIXER_IF(general_mixer_if)->data,
				 i);
	reread_inputs_cb(GTK_WIDGET(general_mixer_if), button);
	gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, TRUE, 0);
	gtk_widget_show(button);

	snprintf(buf, sizeof(buf), "%d", i);
	slider =
	    slider_int_new(buf, &(general_mixer_if->data->vol[i]), 0, 64,
			   1);
	gtk_box_pack_start(GTK_BOX(vbox), slider, TRUE, TRUE, 0);
	gtk_widget_show(slider);

	button = gtk_toggle_button_new_with_label("Mute");
	gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 2);
	gtk_toggle_button_set_state(GTK_TOGGLE_BUTTON(button),
				    general_mixer_if->data->mute[i]);
	g_signal_connect(G_OBJECT(button), "clicked",
			 G_CALLBACK(general_mixer_if_mute_cb),
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
