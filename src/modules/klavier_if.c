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
#include <gdk/gdkkeysyms.h>

#include "klav.h"
#include "aube.h"
#include "klavier_if.h"
#include "gtkaube.h"

extern klavier *klavier_new(void);

void klavkey_press_event(GtkWidget * widget, gint key, gpointer data);
void klavkey_release_event(GtkWidget * widget, gint key, gpointer data);
void klavkey_key_press_event(GtkWidget * widget, GdkEventKey * event,
			     gpointer data);
void klavkey_key_release_event(GtkWidget * widget, GdkEventKey * event,
			       gpointer data);
gint get_note_from_key(guint keyval);

GTKAUBE_BOILERPLATE(KlavierIF,klavier_if)

GtkWidget *klavier_if_new(klavier * mod)
{
	KlavierIF *klavier_if;
	GtkWidget *vbox, *hbox, *hbox2, *hbox3;
	GtkWidget *ebox;
	GtkWidget *frame;
	GtkWidget *button;
	GtkWidget *klav;
	GtkWidget *slider;

	gint start, end, keywidth;
	KlavType klavtype = KLAV_TYPE_NORMAL;
	KlavDir klavdir = KLAV_DIR_HORIZONTAL;

	klavier_if = gtk_type_new(klavier_if_get_type());

	modulewindow_set_module (MODULEWINDOW(klavier_if), (module *)mod);

        hbox2 = MODULEWINDOW(klavier_if)->headbox;

	button = outputlabel_new((module *)mod, 0);
	gtk_box_pack_start(GTK_BOX(hbox2), button, FALSE, FALSE, 4);
	gtk_widget_show(button);

	hbox2 = gtk_hbox_new(FALSE, 5);
	gtk_box_pack_start(GTK_BOX(MODULEWINDOW(klavier_if)->mainbox), hbox2, TRUE, TRUE, 0);
	gtk_widget_show(hbox2);

	/*
	   K L A V I E R 
	 */

	frame = gtk_frame_new(NULL);
	gtk_box_pack_start(GTK_BOX(hbox2), frame, FALSE, TRUE, 2);
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

	slider = slider_int_new("Tune", &mod->tune, 1, 1024, 1);
	gtk_box_pack_start(GTK_BOX(hbox3), slider, TRUE, FALSE, 0);
	gtk_widget_show(slider);

	slider = slider_int_new("Pan", &mod->pan, 0, 32, 0);
	gtk_box_pack_start(GTK_BOX(vbox), slider, FALSE, FALSE, 0);
	gtk_widget_show(slider);


	/*
	   K L A V I E R 
	 */

	frame = gtk_frame_new(NULL);
	gtk_frame_set_shadow_type(GTK_FRAME(frame), GTK_SHADOW_OUT);
	gtk_box_pack_start(GTK_BOX(hbox2), frame, TRUE, TRUE, 0);
	gtk_container_border_width(GTK_CONTAINER(frame), 0);
	gtk_widget_show(frame);

	hbox = gtk_hbox_new(FALSE, 0);
	gtk_container_add(GTK_CONTAINER(frame), hbox);
	gtk_widget_show(hbox);

	ebox = gtk_event_box_new();
	gtk_box_pack_start(GTK_BOX(hbox), ebox, FALSE, FALSE, 0);
	gtk_widget_show(ebox);

	klav = klav_new();
	start = 36;
	end = 95;
	if (klavtype == KLAV_TYPE_NORMAL)
		keywidth = 8;
	else
		keywidth = 10;

	klav_set_range(KLAV(klav), start, end);

	if (klavdir == KLAV_DIR_VERTICAL) {
		gtk_drawing_area_size(GTK_DRAWING_AREA(klav), 60,
				      (end - start + 1) * keywidth);
		klav_set_klav_dir(KLAV(klav), KLAV_DIR_VERTICAL);
	} else {
		gtk_drawing_area_size(GTK_DRAWING_AREA(klav),
				      (end - start + 1) * keywidth, 60);
		klav_set_klav_dir(KLAV(klav), KLAV_DIR_HORIZONTAL);
	}

	klav_set_klav_type(KLAV(klav), klavtype);

	g_signal_connect(G_OBJECT(klav), "klavkey_press",
			 G_CALLBACK(klavkey_press_event), klavier_if);

	g_signal_connect(G_OBJECT(klav), "klavkey_release",
			 G_CALLBACK(klavkey_release_event), klavier_if);

	g_signal_connect(G_OBJECT(ebox), "key_press_event",
			 G_CALLBACK(klavkey_key_press_event), klavier_if);

	g_signal_connect(G_OBJECT(ebox), "key_release_event",
			 G_CALLBACK(klavkey_key_release_event),
			 klavier_if);

	/*
	   gtk_box_pack_start(GTK_BOX(hbox), klav, FALSE, FALSE, 0);
	 */
	gtk_container_add(GTK_CONTAINER(ebox), klav);
	gtk_widget_show(klav);

	return GTK_WIDGET(klavier_if);
}

void klavkey_press_event(GtkWidget * widget, gint key, gpointer data)
{
        KlavierIF * klavier_if = KLAVIER_IF(data);
        klavier * mod = (klavier *)MODULEWINDOW(klavier_if)->module;

	mod->last_klav_key = key;
	klav_press(KLAV(widget), key);
}

void klavkey_release_event(GtkWidget * widget, gint key, gpointer data)
{
	klav_release(KLAV(widget), key);
}

void
klavkey_key_press_event(GtkWidget * widget, GdkEventKey * event,
			gpointer data)
{
        KlavierIF * klavier_if = KLAVIER_IF(data);
        klavier * mod = (klavier *)MODULEWINDOW(klavier_if)->module;
	gint key = get_note_from_key(event->keyval);

	g_print(".");

	if (key >= 0) {
		mod->last_klav_key = key;
		klav_press(KLAV(widget), key);
	}
}

void
klavkey_key_release_event(GtkWidget * widget, GdkEventKey * event,
			  gpointer data)
{
	gint key = get_note_from_key(event->keyval);

	if (key >= 0) {
		klav_release(KLAV(widget), key);
	}
}

gint get_note_from_key(guint keyval)
{
	switch (keyval) {
	case GDK_z:
	case GDK_Z:
		return 0;
		break;
	case GDK_s:
	case GDK_S:
		return 1;
		break;
	case GDK_x:
	case GDK_X:
		return 2;
		break;
	case GDK_d:
	case GDK_D:
		return 3;
		break;
	case GDK_c:
	case GDK_C:
		return 4;
		break;
	case GDK_v:
	case GDK_V:
		return 5;
		break;
	case GDK_g:
	case GDK_G:
		return 6;
		break;
	case GDK_b:
	case GDK_B:
		return 7;
		break;
	case GDK_h:
	case GDK_H:
		return 8;
		break;
	case GDK_n:
	case GDK_N:
		return 9;
		break;
	case GDK_j:
	case GDK_J:
		return 10;
		break;
	case GDK_m:
	case GDK_M:
		return 11;
		break;
	case GDK_comma:
	case GDK_less:
	case GDK_q:
	case GDK_Q:
		return 12;
		break;
	case GDK_l:
	case GDK_L:
	case GDK_2:
	case GDK_at:
		return 13;
		break;
	case GDK_period:
	case GDK_greater:
	case GDK_w:
	case GDK_W:
		return 14;
		break;
	case GDK_semicolon:
	case GDK_colon:
	case GDK_3:
	case GDK_numbersign:
		return 15;
		break;
	case GDK_slash:
	case GDK_question:
	case GDK_e:
	case GDK_E:
		return 16;
		break;
	case GDK_r:
	case GDK_R:
		return 17;
		break;
	case GDK_5:
	case GDK_percent:
		return 18;
		break;
	case GDK_t:
	case GDK_T:
		return 19;
		break;
	case GDK_6:
	case GDK_asciicircum:	/*
				   ????? 
				 */
		return 20;
		break;
	case GDK_y:
	case GDK_Y:
		return 21;
		break;
	case GDK_7:
	case GDK_ampersand:
		return 22;
		break;
	case GDK_u:
	case GDK_U:
		return 23;
		break;
	case GDK_i:
	case GDK_I:
		return 24;
		break;
	case GDK_9:
	case GDK_parenleft:
		return 25;
		break;
	case GDK_o:
	case GDK_O:
		return 26;
		break;
	case GDK_0:
	case GDK_parenright:
		return 27;
		break;
	case GDK_p:
	case GDK_P:
		return 28;
		break;
	case GDK_bracketleft:
	case GDK_braceleft:
		return 29;
		break;
	case GDK_equal:
	case GDK_plus:
		return 30;
		break;
	case GDK_bracketright:
	case GDK_braceright:
		return 31;
		break;
	default:
		return -1;
		break;
	}
}
