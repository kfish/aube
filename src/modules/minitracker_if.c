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
#include "outputlabel.h"
#include "minitracker_if.h"
#include "slider.h"


extern minitracker *minitracker_new(void);

extern int tick;
extern char *note_names[97];

static void minitracker_if_class_init(MinitrackerIFClass * klass);
static void minitracker_if_init(MinitrackerIF * b);
GtkWidget *minitracker_if_new(minitracker * mod);

void minitracker_if_update_tracker(GtkWidget * widget, gpointer data);
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

GType minitracker_if_get_type(void)
{
	static GType b_type = 0;

	if (!b_type) {
		static const GTypeInfo b_info = {
			sizeof(MinitrackerIFClass),
			NULL,	/* base_init */
			NULL,	/* base_finalise */
			(GClassInitFunc) minitracker_if_class_init,
			NULL,	/* class_finalize */
			NULL,	/* class_data */
			sizeof(MinitrackerIF),
			0,	/* n_preallocs */
			(GInstanceInitFunc) minitracker_if_init,
		};

		b_type = g_type_register_static(MODULEWINDOW_TYPE,
						"MinitrackerIF",
						&b_info, 0);
	}
	return b_type;
}

static void minitracker_if_class_init(MinitrackerIFClass * klass)
{

}

static void minitracker_if_init(MinitrackerIF * minitracker_if)
{
}

GtkWidget *minitracker_if_new(minitracker * mod)
{
	MinitrackerIF *minitracker_if;
	GtkWidget *vbox, *hbox, *hbox2, *hbox3;
	GtkWidget *frame;
	GtkWidget *button;
	GtkWidget *slider;
	GSList *rbgroup;
	GtkWidget *scrolled;
	gchar *titles[] = { "", "Note", "Vol", "Pan" };

	minitracker_if = gtk_type_new(minitracker_if_get_type());

	modulewindow_set_module (MODULEWINDOW(minitracker_if), (module *)mod);

	hbox2 = MODULEWINDOW(minitracker_if)->headbox;

	button = outputlabel_new((module *)mod, 0);
	gtk_box_pack_start(GTK_BOX(hbox2), button, FALSE, FALSE, 4);
	gtk_widget_show(button);

	hbox = gtk_hbox_new(FALSE, 5);
	gtk_box_pack_start(GTK_BOX(MODULEWINDOW(minitracker_if)->mainbox), hbox, FALSE, FALSE, 0);
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

	slider = slider_int_new("Vol", &mod->vol, 0, 64, 1);
	gtk_box_pack_start(GTK_BOX(hbox3), slider, FALSE, FALSE, 0);
	gtk_widget_show(slider);

	slider = slider_int_new("Tune", &mod->tune, 1, 1024, 1);
	gtk_box_pack_start(GTK_BOX(hbox3), slider, FALSE, FALSE, 0);
	gtk_widget_show(slider);

	slider = slider_int_new("Pan", &mod->pan, 0, 32, 0);
	gtk_box_pack_start(GTK_BOX(vbox), slider, FALSE, FALSE, 0);
	gtk_widget_show(slider);


	vbox = gtk_vbox_new(FALSE, 2);
	gtk_box_pack_start(GTK_BOX(hbox), vbox, FALSE, FALSE, 1);
	gtk_widget_show(vbox);

	button = gtk_radio_button_new_with_label(NULL, "One note");
	gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 0);
	g_signal_connect(G_OBJECT(button), "clicked",
			 G_CALLBACK(seq_play_once_cb), minitracker_if);
	gtk_widget_show(button);
	rbgroup = gtk_radio_button_group(GTK_RADIO_BUTTON(button));

	button = gtk_radio_button_new_with_label(rbgroup, "Straight 4/4");
	gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 0);
	g_signal_connect(G_OBJECT(button), "clicked",
			 G_CALLBACK(seq_play_44_cb), minitracker_if);
	gtk_widget_show(button);
	rbgroup = gtk_radio_button_group(GTK_RADIO_BUTTON(button));

	button = gtk_radio_button_new_with_label(rbgroup, "Random 8/8");
	gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 0);
	g_signal_connect(G_OBJECT(button), "clicked",
			 G_CALLBACK(seq_chaos8_cb), minitracker_if);
	gtk_widget_show(button);
	rbgroup = gtk_radio_button_group(GTK_RADIO_BUTTON(button));

	button = gtk_radio_button_new_with_label(rbgroup, "Random 16/16");
	gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 0);
	g_signal_connect(G_OBJECT(button), "clicked",
			 G_CALLBACK(chaos_cb), minitracker_if);
	gtk_widget_show(button);

	vbox = gtk_vbox_new(FALSE, 2);
	gtk_box_pack_start(GTK_BOX(hbox), vbox, FALSE, FALSE, 0);
	gtk_widget_show(vbox);

	hbox3 = gtk_hbox_new(FALSE, 2);
	gtk_box_pack_start(GTK_BOX(vbox), hbox3, FALSE, FALSE, 0);
	gtk_widget_show(hbox3);

	button = gtk_button_new_with_label("<<");
	gtk_box_pack_start(GTK_BOX(hbox3), button, FALSE, FALSE, 0);
	g_signal_connect(G_OBJECT(button), "clicked",
			 G_CALLBACK(lshift_cb), minitracker_if);
	gtk_widget_show(button);

	button = gtk_button_new_with_label("Restart");
	gtk_box_pack_start(GTK_BOX(hbox3), button, FALSE, FALSE, 0);
	g_signal_connect(G_OBJECT(button), "clicked",
			 G_CALLBACK(restart_cb), minitracker_if);
	gtk_widget_show(button);

	button = gtk_button_new_with_label(">>");
	gtk_box_pack_start(GTK_BOX(hbox3), button, FALSE, FALSE, 0);
	g_signal_connect(G_OBJECT(button), "clicked",
			 G_CALLBACK(rshift_cb), minitracker_if);
	gtk_widget_show(button);

	hbox3 = gtk_hbox_new(FALSE, 2);
	gtk_box_pack_start(GTK_BOX(vbox), hbox3, TRUE, FALSE, 0);
	gtk_widget_show(hbox3);

	button = gtk_button_new_with_label("-1");
	gtk_box_pack_start(GTK_BOX(hbox3), button, FALSE, FALSE, 0);
	g_signal_connect(G_OBJECT(button), "clicked",
			 G_CALLBACK(seq_transpose_d1_cb), minitracker_if);
	gtk_widget_show(button);

	button = gtk_button_new_with_label("+1");
	gtk_box_pack_start(GTK_BOX(hbox3), button, FALSE, FALSE, 0);
	g_signal_connect(G_OBJECT(button), "clicked",
			 G_CALLBACK(seq_transpose_u1_cb), minitracker_if);
	gtk_widget_show(button);

	hbox3 = gtk_hbox_new(FALSE, 2);
	gtk_box_pack_start(GTK_BOX(vbox), hbox3, FALSE, FALSE, 0);
	gtk_widget_show(hbox3);

	button = gtk_button_new_with_label("-12");
	gtk_box_pack_start(GTK_BOX(hbox3), button, FALSE, FALSE, 0);
	g_signal_connect(G_OBJECT(button), "clicked",
			 G_CALLBACK(seq_transpose_d12_cb), minitracker_if);
	gtk_widget_show(button);

	button = gtk_button_new_with_label("+12");
	gtk_box_pack_start(GTK_BOX(hbox3), button, FALSE, FALSE, 0);
	g_signal_connect(G_OBJECT(button), "clicked",
			 G_CALLBACK(seq_transpose_u12_cb), minitracker_if);
	gtk_widget_show(button);


	/*
	   P O R T A M E N T O 
	 */

	slider = slider_int_new("Portamento", &mod->portamento, 0, 64, 1);
	gtk_box_pack_start(GTK_BOX(hbox), slider, TRUE, FALSE, 0);
	gtk_widget_show(slider);

	/*
	   T R A C K E R 
	 */

	scrolled = gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled),
				       GTK_POLICY_AUTOMATIC,
				       GTK_POLICY_ALWAYS);
	gtk_box_pack_start(GTK_BOX(MODULEWINDOW(minitracker_if)->mainbox), scrolled, TRUE, TRUE, 1);
	gtk_widget_show(scrolled);

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
	gtk_clist_set_column_justification(GTK_CLIST(minitracker_if->tl),
					   1, GTK_JUSTIFY_CENTER);
	gtk_container_add(GTK_CONTAINER(scrolled), minitracker_if->tl);


	minitracker_if_update_tracker(NULL, minitracker_if);

	gtk_widget_show(minitracker_if->tl);

	return GTK_WIDGET(minitracker_if);
}

void minitracker_if_update_tracker(GtkWidget * widget, gpointer data)
{
	MinitrackerIF *tracker = MINITRACKER_IF(data);
        minitracker * mod = (minitracker *)MODULEWINDOW(tracker)->module;
	gchar *title[4];
	gchar buf0[4];
	gchar buf1[8];
	gchar buf2[4];
	gchar buf3[4];
	int i;

	gtk_clist_freeze(GTK_CLIST(tracker->tl));
	gtk_clist_clear(GTK_CLIST(tracker->tl));
	title[0] = buf0;
	title[1] = buf1;
	title[2] = buf2;
	title[3] = buf3;
	for (i = 0; i < SEQ_LENGTH; i++) {
		snprintf(title[0], sizeof(title[0]), "%02d", i);
		/*    snprintf(title[1], sizeof (title[1]), "%d", mod->sequence[i].note); */
		snprintf(title[1], sizeof(title[1]), "%s", note_names[mod->sequence[i].note]);
		snprintf(title[2], sizeof(title[2]), "%02d", 0);
		snprintf(title[3], sizeof(title[3]), "%02d", 0);

		gtk_clist_append(GTK_CLIST(tracker->tl),
				 (gchar **) & title);
	}
	gtk_clist_thaw(GTK_CLIST(tracker->tl));
}

void seq_toggle_cb(GtkWidget * widget, gpointer data)
{
	seq_toggle((minitracker *) data);
}

void restart_cb(GtkWidget * widget, gpointer data)
{
	seq_restart((minitracker *) data);
}

void chaos_cb(GtkWidget * widget, gpointer data)
{
	MinitrackerIF *minitracker_if = MINITRACKER_IF(data);
        minitracker * mod = (minitracker *)MODULEWINDOW(minitracker_if)->module;

	chaos(mod);
	minitracker_if_update_tracker(NULL, minitracker_if);
}

void seq_chaos8_cb(GtkWidget * widget, gpointer data)
{
	MinitrackerIF *minitracker_if = MINITRACKER_IF(data);
        minitracker * mod = (minitracker *)MODULEWINDOW(minitracker_if)->module;

	seq_chaos8(mod);
	minitracker_if_update_tracker(NULL, minitracker_if);
}

void seq_play_once_cb(GtkWidget * widget, gpointer data)
{
	MinitrackerIF *minitracker_if = MINITRACKER_IF(data);
        minitracker * mod = (minitracker *)MODULEWINDOW(minitracker_if)->module;

	seq_play_once(mod);
	minitracker_if_update_tracker(NULL, minitracker_if);
}

void seq_play_44_cb(GtkWidget * widget, gpointer data)
{
	MinitrackerIF *minitracker_if = MINITRACKER_IF(data);
        minitracker * mod = (minitracker *)MODULEWINDOW(minitracker_if)->module;

	seq_play_44(mod);
	minitracker_if_update_tracker(NULL, minitracker_if);
}

void seq_transpose_u12_cb(GtkWidget * widget, gpointer data)
{
	MinitrackerIF *minitracker_if = MINITRACKER_IF(data);
        minitracker * mod = (minitracker *)MODULEWINDOW(minitracker_if)->module;

	seq_transpose_u12(mod);
	minitracker_if_update_tracker(NULL, minitracker_if);
}

void seq_transpose_d12_cb(GtkWidget * widget, gpointer data)
{
	MinitrackerIF *minitracker_if = MINITRACKER_IF(data);
        minitracker * mod = (minitracker *)MODULEWINDOW(minitracker_if)->module;

	seq_transpose_d12(mod);
	minitracker_if_update_tracker(NULL, minitracker_if);
}

void seq_transpose_u1_cb(GtkWidget * widget, gpointer data)
{
	MinitrackerIF *minitracker_if = MINITRACKER_IF(data);
        minitracker * mod = (minitracker *)MODULEWINDOW(minitracker_if)->module;

	seq_transpose_u1(mod);
	minitracker_if_update_tracker(NULL, minitracker_if);
}

void seq_transpose_d1_cb(GtkWidget * widget, gpointer data)
{
	MinitrackerIF *minitracker_if = MINITRACKER_IF(data);
        minitracker * mod = (minitracker *)MODULEWINDOW(minitracker_if)->module;

	seq_transpose_d1(mod);
	minitracker_if_update_tracker(NULL, minitracker_if);
}

void lshift_cb(GtkWidget * widget, gpointer data)
{
	shift_left((minitracker *) data);
}

void rshift_cb(GtkWidget * widget, gpointer data)
{
	shift_right((minitracker *) data);
}
