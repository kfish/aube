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

#include "xfader_if.h"
#include "aube.h"
#include "master.h"
#include "slider.h"
#include "inputoption.h"
#include "outputlabel.h"

static void xfader_if_class_init(XFaderIFClass * klass);
static void xfader_if_init(XFaderIF * b);
GtkWidget *xfader_if_new(xfader * mod);

GType xfader_if_get_type(void)
{
	static GType b_type = 0;

	if (!b_type) {
		static const GTypeInfo b_info = {
			sizeof(XFaderIFClass),
			NULL,	/* base_init */
			NULL,	/* base_finalise */
			(GClassInitFunc) xfader_if_class_init,
			NULL,	/* class_finalize */
			NULL,	/* class_data */
			sizeof(XFaderIF),
			0,	/* n_preallocs */
			(GInstanceInitFunc) xfader_if_init,
		};

		b_type = g_type_register_static(MODULEWINDOW_TYPE,
						"XFaderIF", &b_info, 0);
	}
	return b_type;
}

static void xfader_if_class_init(XFaderIFClass * klass)
{

}

static void xfader_if_init(XFaderIF * xfader_if)
{
}

GtkWidget *xfader_if_new(xfader * mod)
{
	XFaderIF *xfader_if;
	GtkWidget *vbox, *vbox2, *hbox, *hbox2;
	GtkWidget *frame;
	GtkWidget *button;
	GtkWidget *slider;

	xfader_if = gtk_type_new(xfader_if_get_type());

	modulewindow_set_module (MODULEWINDOW(xfader_if), (module *)mod);

	xfader_if->data = mod;

	vbox2 = MODULEWINDOW(xfader_if)->mainbox;
	hbox = MODULEWINDOW(xfader_if)->headbox;

	button = outputlabel_new((module *) XFADER_IF(xfader_if)->data, 0);
	gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 0);
	gtk_widget_show(button);

	hbox = gtk_hbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox2), hbox, FALSE, TRUE, 0);
	gtk_widget_show(hbox);

	button =
	    inputoption_new((char *) "0:",
			    (module *) XFADER_IF(xfader_if)->data, 0);
	gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, TRUE, 0);
	gtk_widget_show(button);

	button =
	    inputoption_new((char *) "1:",
			    (module *) XFADER_IF(xfader_if)->data, 1);
	gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, TRUE, 0);
	gtk_widget_show(button);


	frame = gtk_frame_new(NULL);
	gtk_box_pack_start(GTK_BOX(vbox2), frame, TRUE, TRUE, 0);
	/* gtk_container_border_width(GTK_CONTAINER(frame), 4); */
	gtk_widget_show(frame);

	vbox = gtk_vbox_new(FALSE, 0);
	gtk_container_add(GTK_CONTAINER(frame), vbox);
	gtk_widget_show(vbox);

	hbox2 = gtk_hbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), hbox2, TRUE, TRUE, 0);
	gtk_widget_show(hbox2);

	slider =
	    slider_int_new("Vol0", &(xfader_if->data->vol0), 0, 64, 1);
	gtk_box_pack_start(GTK_BOX(hbox2), slider, TRUE, TRUE, 0);
	gtk_widget_show(slider);

	slider =
	    slider_int_new("Vol1", &(xfader_if->data->vol1), 0, 64, 1);
	gtk_box_pack_start(GTK_BOX(hbox2), slider, TRUE, TRUE, 0);
	gtk_widget_show(slider);

	slider =
	    slider_int_new("Fader", &(xfader_if->data->fader), 0, 64, 0);
	gtk_box_pack_start(GTK_BOX(vbox), slider, FALSE, TRUE, 0);
	gtk_widget_show(slider);

	return GTK_WIDGET(xfader_if);
}
