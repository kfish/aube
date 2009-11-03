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

#include <strings.h>
#include <gtk/gtk.h>

#include "none_config.h"
#include "aube.h"
#include "master.h"
#include "modulemenu.h"
#include "opsmenu.h"
#include "slider.h"
#include "inputoption.h"

extern int tick;

extern GtkWidget *master_daddy;

static void none_config_class_init(NoneConfigClass * class);
static void none_config_init(NoneConfig * b);
void none_config_destroy(GtkWidget * widget, gpointer data);

/*
   this is a callback for module_class's 
 */
void none_config_cb(module * u)
{
	GtkWidget *nc;

	nc = none_config_new(u);
	gtk_widget_show(nc);
}

GType none_config_get_type(void)
{
	static GType b_type = 0;

	if (!b_type) {
		static const GTypeInfo b_info = {
			sizeof(NoneConfigClass),
			NULL,	/* base_init */
			NULL,	/* base_finalise */
			(GClassInitFunc) none_config_class_init,
			NULL,	/* class_finalize */
			NULL,	/* class_data */
			sizeof(NoneConfig),
			0,	/* n_preallocs */
			(GInstanceInitFunc) none_config_init,
		};

		b_type = g_type_register_static(GTK_TYPE_WINDOW,
						"NoneConfig", &b_info, 0);
	}
	return b_type;
}

static void none_config_class_init(NoneConfigClass * class)
{

}

static void none_config_init(NoneConfig * none_config)
{

}

GtkWidget *none_config_new(module * u)
{
	NoneConfig *none_config;
	GtkWidget *vbox;
	GtkWidget *frame;
	GtkWidget *widget;

	none_config = gtk_type_new(none_config_get_type());

	gtk_window_set_title(GTK_WINDOW(none_config), u->u_label);

	gtk_container_border_width(GTK_CONTAINER(none_config), 1);

#if 0
	/*
	   when the window is given the "delete_event" signal - this is
	   * given by the window manager - usually the close option or on the
	   * titlebar - we ask it to call the delete_event() function
	   * as defined above. The data passed to the callback function is
	   * NULL and is ignored in the callback. 
	 */
	g_signal_connect(G_OBJECT(none_config), "delete_event",
			 G_CALLBACK(delete_event), NULL);

	/*
	   here we connect the "destroy" event to a signal handler.
	   * This event occurs when we call gtk_widget_destroy() on the
	   * window, or if we return "TRUE" in the "delete_event" callback. 
	 */
	g_signal_connect(G_OBJECT(none_config), "destroy",
			 G_CALLBACK(destroy), NULL);
#endif

	frame = gtk_frame_new(u->u_label);
	gtk_container_add(GTK_CONTAINER(none_config), frame);
	gtk_widget_show(frame);

	vbox = gtk_vbox_new(FALSE, 1);
	gtk_container_add(GTK_CONTAINER(frame), vbox);
	gtk_widget_show(vbox);

	widget = gtk_label_new("This module has no options.");
	gtk_box_pack_start(GTK_BOX(vbox), widget, FALSE, FALSE, 2);
	gtk_widget_show(widget);

	widget = gtk_button_new_with_label("Ok");
	gtk_box_pack_start(GTK_BOX(vbox), widget, FALSE, FALSE, 2);
	g_signal_connect(G_OBJECT(widget), "clicked",
			 G_CALLBACK(none_config_destroy), none_config);
	gtk_widget_show(widget);

	return GTK_WIDGET(none_config);
}

void none_config_destroy(GtkWidget * widget, gpointer data)
{
	gtk_widget_destroy(GTK_WIDGET(data));
}
