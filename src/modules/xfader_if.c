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
#include "xfader_if.h"
#include "gtkaube.h"

GTKAUBE_BOILERPLATE(XFaderIF,xfader_if)

GtkWidget *xfader_if_new(xfader * mod)
{
	XFaderIF *xfader_if;
	GtkWidget *vbox, *vbox2, *hbox, *hbox2;
	GtkWidget *frame;
	GtkWidget *button;
	GtkWidget *slider;

	xfader_if = gtk_type_new(xfader_if_get_type());

	modulewindow_set_module (MODULEWINDOW(xfader_if), (module *)mod);

        gtk_window_set_default_size (GTK_WINDOW(xfader_if), 0, 320);

	vbox2 = MODULEWINDOW(xfader_if)->mainbox;
	hbox = MODULEWINDOW(xfader_if)->headbox;

	button = outputlabel_new((module *)mod, 0);
	gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 0);
	gtk_widget_show(button);

	hbox = gtk_hbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox2), hbox, FALSE, TRUE, 0);
	gtk_widget_show(hbox);

	button =
	    inputoption_new((char *) "0:", (module *)mod, 0);
	gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, TRUE, 0);
	gtk_widget_show(button);

	button =
	    inputoption_new((char *) "1:", (module *)mod, 1);
	gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, TRUE, 0);
	gtk_widget_show(button);

	frame = gtk_frame_new(NULL);
	gtk_box_pack_start(GTK_BOX(vbox2), frame, TRUE, TRUE, 0);
	gtk_widget_show(frame);

	vbox = gtk_vbox_new(FALSE, 0);
	gtk_container_add(GTK_CONTAINER(frame), vbox);
	gtk_widget_show(vbox);

	hbox2 = gtk_hbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), hbox2, TRUE, TRUE, 0);
	gtk_widget_show(hbox2);

	slider = slider_int_new("Vol0", &mod->vol0, 0, 64, 1);
	gtk_box_pack_start(GTK_BOX(hbox2), slider, TRUE, TRUE, 0);
	gtk_widget_show(slider);

	slider = slider_int_new("Vol1", &mod->vol1, 0, 64, 1);
	gtk_box_pack_start(GTK_BOX(hbox2), slider, TRUE, TRUE, 0);
	gtk_widget_show(slider);

	slider = slider_int_new("Fader", &mod->fader, 0, 64, 0);
	gtk_box_pack_start(GTK_BOX(vbox), slider, FALSE, TRUE, 0);
	gtk_widget_show(slider);

	return GTK_WIDGET(xfader_if);
}
