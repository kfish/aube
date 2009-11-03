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
#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>

#include "whitenoise_if.h"
#include "aube.h"
#include "master.h"
#include "slider.h"
#include "outputlabel.h"

static void whitenoise_if_class_init(WhitenoiseIFClass * klass);
static void whitenoise_if_init(WhitenoiseIF * b);
GtkWidget *whitenoise_if_new(whitenoise * mod);
void whitenoise_if_hide_cb(GtkWidget * widget, gpointer data);
void whitenoise_if_close_cb(GtkWidget * widget, gpointer data);
void whitenoise_if_onoff_cb(GtkWidget * widget, gpointer data);

GType whitenoise_if_get_type(void)
{
	static GType b_type = 0;

	if (!b_type) {
		static const GTypeInfo b_info = {
			sizeof(WhitenoiseIFClass),
			NULL,	/* base_init */
			NULL,	/* base_finalise */
			(GClassInitFunc) whitenoise_if_class_init,
			NULL,	/* class_finalize */
			NULL,	/* class_data */
			sizeof(WhitenoiseIF),
			0,	/* n_preallocs */
			(GInstanceInitFunc) whitenoise_if_init,
		};

		b_type = g_type_register_static(MODULEWINDOW_TYPE,
						"WhitenoiseIF",
						&b_info, 0);
	}
	return b_type;
}

static void whitenoise_if_class_init(WhitenoiseIFClass * klass)
{

}

static void whitenoise_if_init(WhitenoiseIF * whitenoise_if)
{
}

GtkWidget *whitenoise_if_new(whitenoise * mod)
{
	WhitenoiseIF *ui;
	GtkWidget *vbox;
	GtkWidget *frame;
	GtkWidget *button;
	GtkWidget *slider;

	GtkAccelGroup *accel_group;

	ui = gtk_type_new(whitenoise_if_get_type());

	modulewindow_set_module (MODULEWINDOW(ui), (module *)mod);

	ui->data = mod;

	accel_group = gtk_accel_group_new();
	gtk_window_add_accel_group(GTK_WINDOW(ui), accel_group);

#if 0
	/*
	   when the window is given the "delete_event" signal - this is
	   * given by the window manager - usually the close option or on the
	   * titlebar - we ask it to call the delete_event() function
	   * as defined above. The data passed to the callback function is
	   * NULL and is ignored in the callback. 
	 */
	g_signal_connect(G_OBJECT(whitenoise_if), "delete_event",
			 G_CALLBACK(delete_event), NULL);
#endif

#if 1
	/*
	   here we connect the "destroy" event to a signal handler.
	   * This event occurs when we call gtk_widget_destroy() on the
	   * window, or if we return "TRUE" in the "delete_event" callback. 
	 */
	g_signal_connect(G_OBJECT(ui), "destroy", G_CALLBACK(whitenoise_if_close_cb), ui);
#endif

	button = gtk_toggle_button_new_with_label("On");
	gtk_box_pack_start(GTK_BOX(MODULEWINDOW(ui)->headbox), button, FALSE, FALSE, 1);
	gtk_toggle_button_set_state(GTK_TOGGLE_BUTTON(button), ui->data->module.on);
	g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(whitenoise_if_onoff_cb), mod);
	gtk_widget_show(button);

/* FIXME: funky accel business 
 gtk_accel_group_add (accel_group, GDK_Escape, GDK_NONE, 0,
		       G_OBJECT(button), "clicked");
 */

	button = opsmenu_new((module *)ui->data, GTK_WIDGET(ui), whitenoise_if_hide_cb,
			whitenoise_if_close_cb);
	gtk_box_pack_start(GTK_BOX(MODULEWINDOW(ui)->headbox), button, FALSE, FALSE, 4);
	gtk_widget_show(button);

	button = outputlabel_new((module *)mod, 0);
	gtk_box_pack_start(GTK_BOX(MODULEWINDOW(ui)->headbox), button, FALSE, FALSE, 4);
	gtk_widget_show(button);

        /* Mainbox */
	frame = gtk_frame_new(NULL);
	gtk_box_pack_start(GTK_BOX(MODULEWINDOW(ui)->mainbox), frame, TRUE, TRUE, 1);
	gtk_widget_show(frame);

	vbox = gtk_vbox_new(FALSE, 0);
	gtk_container_add(GTK_CONTAINER(frame), vbox);
	gtk_widget_show(vbox);

	slider = slider_int_new("Vol", &(mod->vol), 0, 64, 1);
	gtk_box_pack_start(GTK_BOX(vbox), slider, TRUE, TRUE, 0);
	gtk_widget_show(slider);

	slider = slider_int_new("Pan", &(mod->pan), 0, 32, 0);
	gtk_box_pack_start(GTK_BOX(vbox), slider, FALSE, TRUE, 0);
	gtk_widget_show(slider);

	return GTK_WIDGET(ui);
}

void whitenoise_if_hide_cb(GtkWidget * widget, gpointer data)
{
	module *u;
	WhitenoiseIF *whitenoise_if;

	whitenoise_if = WHITENOISE_IF(data);
	u = (module *) whitenoise_if->data;
	aube_module_remove_if(u);

	gtk_widget_destroy(GTK_WIDGET(data));
}

void whitenoise_if_close_cb(GtkWidget * widget, gpointer data)
{
	module *u;
	WhitenoiseIF *whitenoise_if;

	whitenoise_if = WHITENOISE_IF(data);
	u = (module *) whitenoise_if->data;
	aube_remove_module(u);

	free((WHITENOISE_IF(data))->data);
	gtk_widget_destroy(GTK_WIDGET(data));
}

void whitenoise_if_onoff_cb(GtkWidget * widget, gpointer data)
{
	aube_module_toggle((module *) data);
}
