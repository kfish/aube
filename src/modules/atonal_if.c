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
#include "atonal_if.h"
#include "gtkaube.h"

extern atonal *atonal_new(void);

extern int tick;
extern char *note_names[97];

void atonal_if_update_at(GtkWidget * widget, gpointer data);
void atonal_if_set_note_cb(GtkWidget * widget, gpointer note);
void at_restart_cb(GtkWidget * widget, gpointer data);
void at_clear_cb(GtkWidget * widget, gpointer data);
void at_chaos_cb(GtkWidget * widget, gpointer data);
void at_play_once_cb(GtkWidget * widget, gpointer data);
void at_play_44_cb(GtkWidget * widget, gpointer data);
void at_transpose_u12_cb(GtkWidget * widget, gpointer data);
void at_transpose_d12_cb(GtkWidget * widget, gpointer data);
void at_transpose_u1_cb(GtkWidget * widget, gpointer data);
void at_transpose_d1_cb(GtkWidget * widget, gpointer data);
void at_lshift_cb(GtkWidget * widget, gpointer data);
void at_rshift_cb(GtkWidget * widget, gpointer data);

GTKAUBE_BOILERPLATE(AtonalIF,atonal_if)

GtkWidget *atonal_if_new(atonal * mod)
{
	AtonalIF *atonal_if;
	GtkWidget *vbox2, *hbox;
	GtkWidget *button;
	GtkWidget *slider;
	gint i;
	gchar buf[4];

	atonal_if = gtk_type_new(atonal_if_get_type());

	modulewindow_set_module (MODULEWINDOW(atonal_if), (module *)mod);

#if 0
	button = gtk_button_new_with_label("Clear");
	gtk_box_pack_start(GTK_BOX(hbox2), button, TRUE, TRUE, 1);
	g_signal_connect(G_OBJECT(button), "clicked",
			 G_CALLBACK(at_clear_cb), atonal_if);
	gtk_widget_show(button);

	button = gtk_button_new_with_label("Chaos");
	gtk_box_pack_start(GTK_BOX(hbox2), button, TRUE, TRUE, 1);
	g_signal_connect(G_OBJECT(button), "clicked",
			 G_CALLBACK(at_chaos_cb), atonal_if);
	gtk_widget_show(button);
#endif

	/* S E Q U E N C E */

	hbox = gtk_hbox_new(FALSE, 2);
	gtk_box_pack_start(GTK_BOX(MODULEWINDOW(atonal_if)->mainbox), hbox, TRUE, TRUE, 1);
	gtk_widget_show(hbox);

#if 0
	button = gtk_label_new("Triggers:");
	gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 3);
	gtk_widget_show(button);
#endif

#if 0
	slider = slider_int_new("", &(mod->vol), 0, 64, 0);
	gtk_box_pack_start(GTK_BOX(hbox), slider, FALSE, FALSE, 0);
	gtk_widget_show(slider);
#endif

	for (i = 0; i < AT_LENGTH; i++) {

		if (i && i % 4 == 0) {
			button = gtk_vseparator_new();
			gtk_box_pack_start(GTK_BOX(hbox), button, FALSE,
					   FALSE, 3);
			gtk_widget_show(button);
		}

		vbox2 = gtk_vbox_new(FALSE, 1);
		gtk_box_pack_start(GTK_BOX(hbox), vbox2, TRUE, TRUE, 0);
		gtk_widget_show(vbox2);

		snprintf(buf, 4, "%d", i);
		slider =
		    slider_int_new(buf,
				   &(mod->sequence[i].note),
				   110, 660, 1);
		gtk_box_pack_start(GTK_BOX(vbox2), slider, TRUE, TRUE, 0);
		gtk_widget_show(slider);

		button = gtk_toggle_button_new();
		gtk_box_pack_start(GTK_BOX(vbox2), button, FALSE, FALSE,
				   1);
		gtk_widget_set_usize(button, 10, 10);
		gtk_toggle_button_set_state(GTK_TOGGLE_BUTTON(button),
					    (mod->sequence[i].
					     trigger == 1));
		g_signal_connect(G_OBJECT(button), "clicked",
				 G_CALLBACK(atonal_if_set_note_cb),
				 atonal_if);
		gtk_widget_show(button);
		atonal_if->buttons[i].button = button;
	}

/*
    button = outputlabel_new((module *)mod, j);
    gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 1);
    gtk_widget_show(button);
*/


	return GTK_WIDGET(atonal_if);
}

void atonal_if_update_at(GtkWidget * widget, gpointer data)
{
	AtonalIF *atonal_if = ATONAL_IF(data);
        atonal * mod = (atonal *)MODULEWINDOW(atonal_if)->module;
	int i;

	for (i = 0; i < AT_LENGTH; i++) {
		g_signal_handlers_block_matched(atonal_if->buttons[i].
						button,
						G_SIGNAL_MATCH_DATA, 0, 0,
						NULL, NULL, atonal_if);
		gtk_toggle_button_set_state(GTK_TOGGLE_BUTTON(atonal_if->buttons[i].button),
					    (mod->sequence[i].trigger == 1));
		g_signal_handlers_unblock_matched(atonal_if->buttons[i].button,
						  G_SIGNAL_MATCH_DATA, 0,
						  0, NULL, NULL,
						  atonal_if);
	}
}

void atonal_if_set_note_cb(GtkWidget * widget, gpointer data)
{
	AtonalIF *atonal_if = ATONAL_IF(data);
        atonal * mod = (atonal *)MODULEWINDOW(atonal_if)->module;
	int i;

	for (i = 0; i < AT_LENGTH; i++) {
		if (atonal_if->buttons[i].button == widget) {
			mod->sequence[i].trigger = 1 - mod->sequence[i].trigger;
			break;
		}
	}

	atonal_if_update_at(NULL, atonal_if);
}

void at_restart_cb(GtkWidget * widget, gpointer data)
{
	at_restart((atonal *) data);
}

void at_clear_cb(GtkWidget * widget, gpointer data)
{
	AtonalIF *atonal_if = ATONAL_IF(data);
        atonal * mod = (atonal *)MODULEWINDOW(atonal_if)->module;

	at_clear(mod);
	atonal_if_update_at(NULL, atonal_if);
}

void at_chaos_cb(GtkWidget * widget, gpointer data)
{
	AtonalIF *atonal_if = ATONAL_IF(data);
        atonal * mod = (atonal *)MODULEWINDOW(atonal_if)->module;

	at_chaos(mod);
	atonal_if_update_at(NULL, atonal_if);
}

void at_play_once_cb(GtkWidget * widget, gpointer data)
{
	AtonalIF *atonal_if = ATONAL_IF(data);
        atonal * mod = (atonal *)MODULEWINDOW(atonal_if)->module;

	at_play_once(mod);
	atonal_if_update_at(NULL, atonal_if);
}

void at_play_44_cb(GtkWidget * widget, gpointer data)
{
	AtonalIF *atonal_if = ATONAL_IF(data);
        atonal * mod = (atonal *)MODULEWINDOW(atonal_if)->module;

	at_play_44(mod);
	atonal_if_update_at(NULL, atonal_if);
}

void at_transpose_u12_cb(GtkWidget * widget, gpointer data)
{
	AtonalIF *atonal_if = ATONAL_IF(data);
        atonal * mod = (atonal *)MODULEWINDOW(atonal_if)->module;

	at_transpose_u12(mod);
	atonal_if_update_at(NULL, atonal_if);
}

void at_transpose_d12_cb(GtkWidget * widget, gpointer data)
{
	AtonalIF *atonal_if = ATONAL_IF(data);
        atonal * mod = (atonal *)MODULEWINDOW(atonal_if)->module;

	at_transpose_d12(mod);
	atonal_if_update_at(NULL, atonal_if);
}

void at_transpose_u1_cb(GtkWidget * widget, gpointer data)
{
	AtonalIF *atonal_if = ATONAL_IF(data);
        atonal * mod = (atonal *)MODULEWINDOW(atonal_if)->module;

	at_transpose_u1(mod);
	atonal_if_update_at(NULL, atonal_if);
}

void at_transpose_d1_cb(GtkWidget * widget, gpointer data)
{
	AtonalIF *atonal_if = ATONAL_IF(data);
        atonal * mod = (atonal *)MODULEWINDOW(atonal_if)->module;

	at_transpose_d1(mod);
	atonal_if_update_at(NULL, atonal_if);
}

void at_lshift_cb(GtkWidget * widget, gpointer data)
{
	at_shift_left((atonal *) data);
}

void at_rshift_cb(GtkWidget * widget, gpointer data)
{
	at_shift_right((atonal *) data);
}
