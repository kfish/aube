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

#include "minimaube_if.h"
#include "aube.h"
#include "modulemenu.h"
#include "opsmenu.h"
#include "slider.h"
#include "inputoption.h"
#include "outputlabel.h"

extern int tick;

extern GtkWidget *master_daddy;

void minimaube_if_onoff_cb(GtkWidget * widget, gpointer data);
void minimaube_if_add_input_cb(GtkWidget * widget, gpointer data);
void minimaube_if_remove_input_cb(GtkWidget * widget, gpointer data);
void minimaube_if_add_input(MiniMaubeIF * minimaube_if, int i);
void minimaube_if_remove_input(MiniMaubeIF * minimaube_if, int i);

GType minimaube_if_get_type(void)
{
	static GType b_type = 0;

	if (!b_type) {
		static const GTypeInfo b_info = {
			sizeof(MiniMaubeIFClass),
			NULL,	/* base_init */
			NULL,	/* base_finalise */
			NULL,   /* class_init */
			NULL,	/* class_finalize */
			NULL,	/* class_data */
			sizeof(MiniMaubeIF),
			0,	/* n_preallocs */
			NULL    /* init */
		};

		b_type = g_type_register_static(MODULEWINDOW_TYPE,
						"MiniMaubeIF", &b_info, 0);
	}
	return b_type;
}

GtkWidget *minimaube_if_new(minimaube * mod)
{
	MiniMaubeIF *minimaube_if;
	GtkWidget *hbox;
	GtkWidget *widget;
	int i;

	minimaube_if = gtk_type_new(minimaube_if_get_type());

	modulewindow_set_module (MODULEWINDOW(minimaube_if), (module *)mod);

	hbox = MODULEWINDOW(minimaube_if)->headbox;

	widget = gtk_button_new_with_label("Add");
	gtk_box_pack_start(GTK_BOX(hbox), widget, FALSE, FALSE, 2);
	g_signal_connect(G_OBJECT(widget), "clicked",
			 G_CALLBACK(minimaube_if_add_input_cb),
			 minimaube_if);
	gtk_widget_show(widget);

	widget = gtk_button_new_with_label("Remove");
	gtk_box_pack_start(GTK_BOX(hbox), widget, FALSE, FALSE, 2);
	g_signal_connect(G_OBJECT(widget), "clicked",
			 G_CALLBACK(minimaube_if_remove_input_cb),
			 minimaube_if);
	gtk_widget_show(widget);

	for (i = 0; i < mod->nr_active_channels; i++) {
		minimaube_if_add_input(minimaube_if, i);
	}

	return GTK_WIDGET(minimaube_if);
}

void minimaube_if_add_input_cb(GtkWidget * widget, gpointer data)
{
	MiniMaubeIF *minimaube_if = MINIMAUBE_IF(data);
        minimaube * mod = (minimaube *)MODULEWINDOW(minimaube_if)->module;
	int i;

	if ((i = minimaube_add_input(mod)) > 0) {
		minimaube_if_add_input(minimaube_if, i);
	}
}

void minimaube_if_remove_input_cb(GtkWidget * widget, gpointer data)
{
	MiniMaubeIF *minimaube_if = MINIMAUBE_IF(data);
        minimaube * mod = (minimaube *)MODULEWINDOW(minimaube_if)->module;
	int i;

	if ((i = minimaube_remove_input(mod)) > 0) {
		minimaube_if_remove_input(minimaube_if, i);
	}
}

void minimaube_if_add_input(MiniMaubeIF * minimaube_if, int i)
{
        minimaube * mod = (minimaube *)MODULEWINDOW(minimaube_if)->module;
	char buf[8];
	GtkWidget *hbox;
	GtkWidget *widget;

	hbox = gtk_hbox_new(FALSE, 1);
	gtk_box_pack_start(GTK_BOX(MODULEWINDOW(minimaube_if)->mainbox), hbox, FALSE, TRUE,
			   1);
	gtk_widget_show(hbox);

	snprintf(buf, sizeof(buf), "%d:", i);
	widget = gtk_label_new(buf);
	gtk_box_pack_start(GTK_BOX(hbox), widget, FALSE, TRUE, 1);
	gtk_widget_show(widget);

	widget = inputoption_new((char *) "Smpl:", (module *)mod, i * 2);
	reread_inputs_cb(GTK_WIDGET(minimaube_if), widget);
	gtk_box_pack_start(GTK_BOX(hbox), widget, TRUE, TRUE, 1);
	gtk_widget_show(widget);

	widget = inputoption_new((char *) "Seq:", (module *)mod, i * 2 + 1);
	reread_inputs_cb(GTK_WIDGET(minimaube_if), widget);
	gtk_box_pack_start(GTK_BOX(hbox), widget, TRUE, TRUE, 1);
	gtk_widget_show(widget);

	widget = outputlabel_new((module *)mod, i);
	gtk_box_pack_start(GTK_BOX(hbox), widget, FALSE, TRUE, 1);
	gtk_widget_show(widget);

	minimaube_if->hboxes[i] = hbox;

#if 0
	char buf[8];
	GtkWidget *vbox, *button, *slider;

	vbox = gtk_vbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(minimaube_if->hbox), vbox, TRUE, FALSE,
			   0);
	gtk_widget_show(vbox);

	snprintf(buf, sizeof(buf), "%d:", i);
	button = inputoption_new(buf,
				 (module *) MINIMAUBE_IF(minimaube_if)->
				 data, i);
	reread_inputs_cb(GTK_WIDGET(minimaube_if), button);
	gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 0);
	gtk_widget_show(button);

	snprintf(buf, sizeof(buf), "%d", i);
	slider = slider_int_new(buf, &mod->vol[i], 0, 64, 1);
	gtk_box_pack_start(GTK_BOX(vbox), slider, TRUE, FALSE, 0);
	gtk_widget_show(slider);

	minimaube_if->vboxes[i] = vbox;
#endif
}

void minimaube_if_remove_input(MiniMaubeIF * minimaube_if, int i)
{
	if (minimaube_if->hboxes[i] != NULL) {
		gtk_widget_destroy(minimaube_if->hboxes[i]);
		minimaube_if->hboxes[i] = NULL;
	}
}
