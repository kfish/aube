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
#include "panner4_if.h"
#include "gtkaube.h"

GTKAUBE_BOILERPLATE(Panner4IF,panner4_if)

GtkWidget *panner4_if_new(panner4 * mod)
{
	Panner4IF *panner4_if;
	GtkWidget *vbox, *vbox2, *hbox, *hbox2, *hbox3;
	GtkWidget *frame;
	GtkWidget *button;
	GtkWidget *slider;

	panner4_if = gtk_type_new(panner4_if_get_type());

	modulewindow_set_module (MODULEWINDOW(panner4_if), (module *)mod);

        gtk_window_set_default_size (GTK_WINDOW(panner4_if), 0, 360);

        vbox2 = MODULEWINDOW(panner4_if)->mainbox;

	hbox = gtk_hbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox2), hbox, FALSE, FALSE, 1);
	gtk_widget_show(hbox);

	button = inputoption_new((char *) "In:", (module *)mod, 0);
	gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, TRUE, 2);
	gtk_widget_show(button);

	hbox = gtk_hbox_new(FALSE, 5);
	gtk_box_pack_start(GTK_BOX(vbox2), hbox, FALSE, TRUE, 0);
	gtk_widget_show(hbox);

	button = outputlabel_new((module *)mod, 0);
	gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, TRUE, 2);
	gtk_widget_show(button);

	button = outputlabel_new((module *)mod, 1);
	gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, TRUE, 2);
	gtk_widget_show(button);

	hbox2 = gtk_hbox_new(FALSE, 5);
	gtk_box_pack_start(GTK_BOX(vbox2), hbox2, TRUE, TRUE, 0);
	gtk_widget_show(hbox2);

	frame = gtk_frame_new(NULL);
	gtk_box_pack_start(GTK_BOX(hbox2), frame, TRUE, TRUE, 0);
	gtk_widget_show(frame);

	hbox = gtk_hbox_new(FALSE, 0);
	gtk_container_add(GTK_CONTAINER(frame), hbox);
	gtk_widget_show(hbox);

	vbox = gtk_vbox_new(FALSE, 5);
	gtk_box_pack_start(GTK_BOX(hbox), vbox, TRUE, TRUE, 0);
	gtk_widget_show(vbox);

	hbox3 = gtk_hbox_new(FALSE, 5);
	gtk_box_pack_start(GTK_BOX(vbox), hbox3, TRUE, TRUE, 0);
	gtk_widget_show(hbox3);

	slider = slider_int_new("Fade", &mod->fade, 0, 32, 1);
	gtk_box_pack_start(GTK_BOX(hbox3), slider, TRUE, FALSE, 0);
	gtk_widget_show(slider);

	slider = slider_int_new("Pan", &mod->pan, 0, 32, 0);
	gtk_box_pack_start(GTK_BOX(vbox), slider, FALSE, TRUE, 0);
	gtk_widget_show(slider);


	return GTK_WIDGET(panner4_if);
}
