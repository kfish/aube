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
#include "drummachine_if.h"
#include "gtkaube.h"

extern drummachine *drummachine_new(void);

void drummachine_if_update_dm(GtkWidget * widget, gpointer data);
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

void dm_shift_left_cb(GtkWidget * widget, gpointer data);
void dm_shift_right_cb(GtkWidget * widget, gpointer data);

void dm_clear_track_cb(GtkWidget * widget, gpointer data);
void dm_shift_track_left_cb(GtkWidget * widget, gpointer data);
void dm_shift_track_right_cb(GtkWidget * widget, gpointer data);

GTKAUBE_BOILERPLATE(DrumMachineIF,drummachine_if)

GtkWidget *drummachine_if_new(drummachine * mod)
{
	DrumMachineIF *drummachine_if;
	GtkWidget *hbox;
	GtkWidget *frame;
	GtkWidget *button, *arrow;
	gint i, j;
	gchar buf[4];

	drummachine_if = gtk_type_new(drummachine_if_get_type());

	modulewindow_set_module (MODULEWINDOW(drummachine_if), (module *)mod);

	frame = gtk_frame_new(NULL);
	gtk_box_pack_start(GTK_BOX(MODULEWINDOW(drummachine_if)->headbox), frame, TRUE, TRUE, 1);
	gtk_frame_set_shadow_type(GTK_FRAME(frame), GTK_SHADOW_IN);
	gtk_container_border_width(GTK_CONTAINER(frame), 1);
	gtk_widget_show(frame);

	hbox = gtk_hbox_new(FALSE, 2);
	gtk_container_add(GTK_CONTAINER(frame), hbox);
	gtk_widget_show(hbox);

	button = gtk_label_new("All tracks:");
	gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 3);
	gtk_widget_show(button);

	button = gtk_button_new_with_label("Clear");
	gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, TRUE, 1);
	g_signal_connect(G_OBJECT(button), "clicked",
			 G_CALLBACK(dm_clear_cb), drummachine_if);
	gtk_widget_show(button);

	button = gtk_button_new_with_label("Chaos");
	gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, TRUE, 1);
	g_signal_connect(G_OBJECT(button), "clicked",
			 G_CALLBACK(dm_chaos_cb), drummachine_if);
	gtk_widget_show(button);

	button = gtk_button_new();
	g_signal_connect(G_OBJECT(button), "clicked",
			 G_CALLBACK(dm_shift_left_cb), drummachine_if);
	gtk_button_set_relief(GTK_BUTTON(button), GTK_RELIEF_NONE);
	gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 0);
	arrow = gtk_arrow_new(GTK_ARROW_LEFT, GTK_SHADOW_OUT);
	gtk_container_add(GTK_CONTAINER(button), arrow);
	gtk_widget_show(arrow);
	gtk_widget_show(button);

	button = gtk_button_new();
	g_signal_connect(G_OBJECT(button), "clicked",
			 G_CALLBACK(dm_shift_right_cb), drummachine_if);
	gtk_button_set_relief(GTK_BUTTON(button), GTK_RELIEF_NONE);
	gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 0);
	arrow = gtk_arrow_new(GTK_ARROW_RIGHT, GTK_SHADOW_OUT);
	gtk_container_add(GTK_CONTAINER(button), arrow);
	gtk_widget_show(arrow);
	gtk_widget_show(button);

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
		gtk_box_pack_start(GTK_BOX(MODULEWINDOW(drummachine_if)->mainbox), hbox, TRUE, TRUE, 1);
		gtk_widget_show(hbox);

		snprintf(buf, 4, "%d:", j);
		button = gtk_label_new(buf);
		gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, FALSE, 3);
		gtk_widget_show(button);

		button = gtk_button_new_with_label("Clear");
		g_object_set_data(G_OBJECT(button), "default",
				  (gpointer) j);
		gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 0);
		g_signal_connect(G_OBJECT(button), "clicked",
				 G_CALLBACK(dm_clear_track_cb),
				 drummachine_if);
		gtk_widget_show(button);

#if 0
		slider = slider_int_new("Vol", &mod->vol, 0, 64, 0);
		gtk_box_pack_start(GTK_BOX(hbox), slider, FALSE, FALSE, 0);
		gtk_widget_show(slider);
#endif

		button = gtk_vseparator_new();
		gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, TRUE, 3);
		gtk_widget_show(button);

		for (i = 0; i < DM_LENGTH; i++) {

			if (i && i % 4 == 0) {
				button = gtk_vseparator_new();
				gtk_box_pack_start(GTK_BOX(hbox), button,
						   TRUE, TRUE, 3);
				gtk_widget_show(button);
			}
			button = gtk_toggle_button_new();
			gtk_box_pack_start(GTK_BOX(hbox), button, TRUE,
					   TRUE, 1);
			gtk_widget_set_usize(button, 10, 10);
			gtk_toggle_button_set_state(GTK_TOGGLE_BUTTON(button),
						    (mod->sequence[j][i].note == 128));
			g_signal_connect(G_OBJECT(button), "clicked",
					 G_CALLBACK
					 (drummachine_if_set_note_cb),
					 drummachine_if);
			gtk_widget_show(button);
			drummachine_if->buttons[j][i].button = button;
		}

		button = gtk_vseparator_new();
		gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, TRUE, 3);
		gtk_widget_show(button);

		button = gtk_button_new();
		g_object_set_data(G_OBJECT(button), "default",
				  (gpointer) j);
		g_signal_connect(G_OBJECT(button), "clicked",
				 G_CALLBACK(dm_shift_track_left_cb),
				 drummachine_if);
		gtk_button_set_relief(GTK_BUTTON(button), GTK_RELIEF_NONE);
		gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 0);
		arrow = gtk_arrow_new(GTK_ARROW_LEFT, GTK_SHADOW_OUT);
		gtk_container_add(GTK_CONTAINER(button), arrow);
		gtk_widget_show(arrow);
		gtk_widget_show(button);

		button = gtk_button_new();
		g_object_set_data(G_OBJECT(button), "default",
				  (gpointer) j);
		g_signal_connect(G_OBJECT(button), "clicked",
				 G_CALLBACK(dm_shift_track_right_cb),
				 drummachine_if);
		gtk_button_set_relief(GTK_BUTTON(button), GTK_RELIEF_NONE);
		gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 0);
		arrow = gtk_arrow_new(GTK_ARROW_RIGHT, GTK_SHADOW_OUT);
		gtk_container_add(GTK_CONTAINER(button), arrow);
		gtk_widget_show(arrow);
		gtk_widget_show(button);

/*
   button = outputlabel_new((module *)mod, j);
   gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 1);
   gtk_widget_show(button);
 */


	}

	return GTK_WIDGET(drummachine_if);
}

void drummachine_if_update_dm(GtkWidget * widget, gpointer data)
{
	DrumMachineIF *drummachine_if = DRUMMACHINE_IF(data);
        drummachine * dm = (drummachine *)MODULEWINDOW(drummachine_if)->module;
	int i, j;

	for (j = 0; j < MAX_CHANNELS; j++) {
		for (i = 0; i < DM_LENGTH; i++) {
			g_signal_handlers_block_matched(drummachine_if->buttons[j][i].button,
							G_SIGNAL_MATCH_DATA,
							0, 0, NULL, NULL,
							drummachine_if);
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(drummachine_if->buttons[j][i].button),
						    (dm->sequence[j][i].note == 128));
			g_signal_handlers_unblock_matched(drummachine_if->buttons[j][i].button,
							  G_SIGNAL_MATCH_DATA,
							  0, 0, NULL, NULL,
							  drummachine_if);
		}
	}
}

void drummachine_if_set_note_cb(GtkWidget * widget, gpointer data)
{
	DrumMachineIF *drummachine_if = DRUMMACHINE_IF(data);
        drummachine * dm = (drummachine *)MODULEWINDOW(drummachine_if)->module;
	int i, j;

	for (j = 0; j < MAX_CHANNELS; j++) {
		for (i = 0; i < DM_LENGTH; i++) {
			if (drummachine_if->buttons[j][i].button == widget) {
				dm->sequence[j][i].note = 128 - dm->sequence[j][i].note;
				break;
			}
		}
	}

	drummachine_if_update_dm(NULL, drummachine_if);
}

void dm_restart_cb(GtkWidget * widget, gpointer data)
{
	dm_restart((drummachine *) data);
}

void dm_clear_cb(GtkWidget * widget, gpointer data)
{
	DrumMachineIF *drummachine_if = DRUMMACHINE_IF(data);
        drummachine * dm = (drummachine *)MODULEWINDOW(drummachine_if)->module;

	dm_clear(dm);
	drummachine_if_update_dm(NULL, drummachine_if);
}

void dm_chaos_cb(GtkWidget * widget, gpointer data)
{
	DrumMachineIF *drummachine_if = DRUMMACHINE_IF(data);
        drummachine * dm = (drummachine *)MODULEWINDOW(drummachine_if)->module;

	dm_chaos(dm);
	drummachine_if_update_dm(NULL, drummachine_if);
}

void dm_shift_left_cb(GtkWidget * widget, gpointer data)
{
	DrumMachineIF *drummachine_if = DRUMMACHINE_IF(data);
        drummachine * dm = (drummachine *)MODULEWINDOW(drummachine_if)->module;
	int j;

	for (j = 0; j < MAX_CHANNELS; j++) {
		dm_shift_track(dm, j, -1);
	}

	drummachine_if_update_dm(NULL, drummachine_if);
}

void dm_shift_right_cb(GtkWidget * widget, gpointer data)
{
	DrumMachineIF *drummachine_if = DRUMMACHINE_IF(data);
        drummachine * dm = (drummachine *)MODULEWINDOW(drummachine_if)->module;
	int j;

	for (j = 0; j < MAX_CHANNELS; j++) {
		dm_shift_track(dm, j, 1);
	}

	drummachine_if_update_dm(NULL, drummachine_if);
}

void dm_clear_track_cb(GtkWidget * widget, gpointer data)
{
	DrumMachineIF *drummachine_if = DRUMMACHINE_IF(data);
        drummachine * dm = (drummachine *)MODULEWINDOW(drummachine_if)->module;
	int j = (int) g_object_get_data(G_OBJECT(widget), "default");

	dm_clear_track(dm, j);

	drummachine_if_update_dm(NULL, drummachine_if);
}

void dm_shift_track_left_cb(GtkWidget * widget, gpointer data)
{
	DrumMachineIF *drummachine_if = DRUMMACHINE_IF(data);
        drummachine * dm = (drummachine *)MODULEWINDOW(drummachine_if)->module;
	int j = (int) g_object_get_data(G_OBJECT(widget), "default");

	dm_shift_track(dm, j, -1);

	drummachine_if_update_dm(NULL, drummachine_if);
}

void dm_shift_track_right_cb(GtkWidget * widget, gpointer data)
{
	DrumMachineIF *drummachine_if = DRUMMACHINE_IF(data);
        drummachine * dm = (drummachine *)MODULEWINDOW(drummachine_if)->module;
	int j = (int) g_object_get_data(G_OBJECT(widget), "default");

	dm_shift_track(dm, j, 1);

	drummachine_if_update_dm(NULL, drummachine_if);
}

void dm_play_once_cb(GtkWidget * widget, gpointer data)
{
	DrumMachineIF *drummachine_if = DRUMMACHINE_IF(data);
        drummachine * dm = (drummachine *)MODULEWINDOW(drummachine_if)->module;

	dm_play_once(dm);
	drummachine_if_update_dm(NULL, drummachine_if);
}

void dm_play_44_cb(GtkWidget * widget, gpointer data)
{
	DrumMachineIF *drummachine_if = DRUMMACHINE_IF(data);
        drummachine * dm = (drummachine *)MODULEWINDOW(drummachine_if)->module;

	dm_play_44(dm);
	drummachine_if_update_dm(NULL, drummachine_if);
}

void dm_transpose_u12_cb(GtkWidget * widget, gpointer data)
{
	DrumMachineIF *drummachine_if = DRUMMACHINE_IF(data);
        drummachine * dm = (drummachine *)MODULEWINDOW(drummachine_if)->module;

	dm_transpose_u12(dm);
	drummachine_if_update_dm(NULL, drummachine_if);
}

void dm_transpose_d12_cb(GtkWidget * widget, gpointer data)
{
	DrumMachineIF *drummachine_if = DRUMMACHINE_IF(data);
        drummachine * dm = (drummachine *)MODULEWINDOW(drummachine_if)->module;

	dm_transpose_d12(dm);
	drummachine_if_update_dm(NULL, drummachine_if);
}

void dm_transpose_u1_cb(GtkWidget * widget, gpointer data)
{
	DrumMachineIF *drummachine_if = DRUMMACHINE_IF(data);
        drummachine * dm = (drummachine *)MODULEWINDOW(drummachine_if)->module;

	dm_transpose_u1(dm);
	drummachine_if_update_dm(NULL, drummachine_if);
}

void dm_transpose_d1_cb(GtkWidget * widget, gpointer data)
{
	DrumMachineIF *drummachine_if = DRUMMACHINE_IF(data);
        drummachine * dm = (drummachine *)MODULEWINDOW(drummachine_if)->module;

	dm_transpose_d1(dm);
	drummachine_if_update_dm(NULL, drummachine_if);
}

void dm_lshift_cb(GtkWidget * widget, gpointer data)
{
	dm_shift_left((drummachine *) data);
}

void dm_rshift_cb(GtkWidget * widget, gpointer data)
{
	dm_shift_right((drummachine *) data);
}
