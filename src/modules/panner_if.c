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

#include "panner_if.h"
#include "aube.h"
#include "modulewindow.h"
#include "master.h"
#include "slider.h"
#include "inputoption.h"
#include "outputlabel.h"

extern int tick;

static void panner_if_class_init(PannerIFClass * klass);
static void panner_if_init(PannerIF * b);
GtkWidget *panner_if_new(panner * mod);
void panner_if_onoff_cb(GtkWidget * widget, gpointer data);

GType panner_if_get_type(void)
{
	static GType b_type = 0;

	if (!b_type) {
		static const GTypeInfo b_info = {
			sizeof(PannerIFClass),
			NULL,	/* base_init */
			NULL,	/* base_finalise */
			(GClassInitFunc) panner_if_class_init,
			NULL,	/* class_finalize */
			NULL,	/* class_data */
			sizeof(PannerIF),
			0,	/* n_preallocs */
			(GInstanceInitFunc) panner_if_init,
		};

		b_type = g_type_register_static(MODULEWINDOW_TYPE,
						"PannerIF", &b_info, 0);
	}
	return b_type;
}

static void panner_if_class_init(PannerIFClass * klass)
{

}

static void panner_if_init(PannerIF * panner_if)
{
}

GtkWidget *panner_if_new(panner * mod)
{
	PannerIF *panner_if;
	GtkWidget *vbox, *vbox2, *hbox, *hbox2;
	GtkWidget *frame;
	GtkWidget *button;
	GtkWidget *slider;

	panner_if = gtk_type_new(panner_if_get_type());

	modulewindow_set_module (MODULEWINDOW(panner_if), (module *)mod);

	panner_if->data = mod;

	vbox2 = MODULEWINDOW(panner_if)->mainbox;
        hbox = MODULEWINDOW(panner_if)->headbox;

	button = outputlabel_new((module *) PANNER_IF(panner_if)->data, 0);
	gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 2);
	gtk_widget_show(button);

	hbox = gtk_hbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox2), hbox, FALSE, FALSE, 0);
	gtk_widget_show(hbox);

	button =
	    inputoption_new((char *) "In:",
			    (module *) PANNER_IF(panner_if)->data, 0);
	gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, TRUE, 2);
	gtk_widget_show(button);

	hbox2 = gtk_hbox_new(FALSE, 5);
	gtk_box_pack_start(GTK_BOX(vbox2), hbox2, TRUE, TRUE, 0);
	gtk_widget_show(hbox2);

	frame = gtk_frame_new(NULL);
	gtk_box_pack_start(GTK_BOX(hbox2), frame, TRUE, TRUE, 0);
/*
   gtk_container_border_width(GTK_CONTAINER(frame), 4);
 */
	gtk_widget_show(frame);

	vbox = gtk_vbox_new(FALSE, 5);
	gtk_container_add(GTK_CONTAINER(frame), vbox);
	gtk_widget_show(vbox);

	slider =
	    slider_int_new("Input", &(panner_if->data->vol), 0, 64, 1);
	gtk_box_pack_start(GTK_BOX(vbox), slider, TRUE, TRUE, 0);
	gtk_widget_show(slider);

	slider = slider_int_new("Pan", &(panner_if->data->pan), 0, 32, 0);
	gtk_box_pack_start(GTK_BOX(vbox), slider, FALSE, TRUE, 0);
	gtk_widget_show(slider);

	return GTK_WIDGET(panner_if);
}
