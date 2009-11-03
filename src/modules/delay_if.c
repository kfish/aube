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

#include "delay_if.h"
#include "aube.h"
#include "master.h"
#include "modulemenu.h"
#include "opsmenu.h"
#include "slider.h"
#include "inputoption.h"
#include "outputlabel.h"

extern int tick;

extern GtkWidget *master_daddy;

static void delay_if_class_init(DelayIFClass * klass);
static void delay_if_init(DelayIF * b);
GtkWidget *delay_if_new(delay * mod);
void delay_if_change_replace_menu_cb(GtkWidget * widget, gpointer data);

GType delay_if_get_type(void)
{
	static GType b_type = 0;

	if (!b_type) {
		static const GTypeInfo b_info = {
			sizeof(DelayIFClass),
			NULL,	/* base_init */
			NULL,	/* base_finalise */
			(GClassInitFunc) delay_if_class_init,
			NULL,	/* class_finalize */
			NULL,	/* class_data */
			sizeof(DelayIF),
			0,	/* n_preallocs */
			(GInstanceInitFunc) delay_if_init,
		};

		b_type = g_type_register_static(MODULEWINDOW_TYPE,
						"DelayIF", &b_info, 0);
	}
	return b_type;
}

static void delay_if_class_init(DelayIFClass * klass)
{

}

static void delay_if_init(DelayIF * delay_if)
{
}

GtkWidget *delay_if_new(delay * mod)
{
	DelayIF *delay_if;
	GtkWidget *vbox, *hbox2, *hbox3;
	GtkWidget *frame;
	GtkWidget *widget;
	GtkWidget *slider;

	delay_if = gtk_type_new(delay_if_get_type());

	modulewindow_set_module (MODULEWINDOW(delay_if), (module *)mod);

	widget = outputlabel_new((module *)mod, 0);
	gtk_box_pack_start(GTK_BOX(MODULEWINDOW(delay_if)->headbox), widget, TRUE, TRUE, 2);
	gtk_widget_show(widget);

	/* I N P U T */

	widget =
	    inputoption_new((char *) "In:", (module *)mod, 0);
	gtk_box_pack_start(GTK_BOX(MODULEWINDOW(delay_if)->mainbox), widget, FALSE, TRUE, 2);
	gtk_widget_show(widget);

	hbox2 = gtk_hbox_new(FALSE, 5);
	gtk_box_pack_start(GTK_BOX(MODULEWINDOW(delay_if)->mainbox), hbox2, TRUE, TRUE, 0);
	gtk_widget_show(hbox2);

	frame = gtk_frame_new(NULL);
	gtk_box_pack_start(GTK_BOX(hbox2), frame, TRUE, TRUE, 1);
	gtk_widget_show(frame);

	vbox = gtk_vbox_new(FALSE, 5);
	gtk_container_add(GTK_CONTAINER(frame), vbox);
	gtk_widget_show(vbox);

	hbox3 = gtk_hbox_new(FALSE, 5);
	gtk_box_pack_start(GTK_BOX(vbox), hbox3, TRUE, TRUE, 0);
	gtk_widget_show(hbox3);

#if 0
	slider = slider_int_new("Input Vol", &mod->input_vol, 0, 64, 1);
	gtk_box_pack_start(GTK_BOX(hbox3), slider, TRUE, FALSE, 0);
	gtk_widget_show(slider);
#endif

	slider = slider_int_new("Delay", &mod->offset, 0,
                                DELAY_BUFFER_LENGTH / 2, 1);
	gtk_box_pack_start(GTK_BOX(hbox3), slider, TRUE, FALSE, 0);
	gtk_widget_show(slider);

	slider = slider_int_new("Delay Vol", &mod->delay_vol, 0, 64, 1);
	gtk_box_pack_start(GTK_BOX(hbox3), slider, TRUE, FALSE, 0);
	gtk_widget_show(slider);

	slider = slider_int_new("Delay Pan", &mod->pan, 0, 32, 0);
	gtk_box_pack_start(GTK_BOX(vbox), slider, FALSE, TRUE, 0);
	gtk_widget_show(slider);

	return GTK_WIDGET(delay_if);
}

void delay_if_change_replace_menu_cb(GtkWidget * widget, gpointer data)
{
        DelayIF * delay_if = DELAY_IF(data);
        module * module = MODULEWINDOW(delay_if)->module;
	GtkWidget *menu;

	menu =
	    modulemenu_new(module,
			   (void *) aube_module_cmp_type,
			   (void *) module_replace_cb);
	gtk_menu_item_remove_submenu(GTK_MENU_ITEM(delay_if->replace_menuitem));
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(delay_if->replace_menuitem), menu);
}
