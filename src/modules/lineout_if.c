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

#include "lineout_if.h"
#include "aube.h"
#include "slider.h"
#include "inputoption.h"
#include "opsmenu.h"

void lineout_if_set_device_dsp_cb(GtkWidget * widget, gpointer data);
void lineout_if_set_device_dsp1_cb(GtkWidget * widget, gpointer data);

GType lineout_if_get_type(void)
{
	static GType b_type = 0;

	if (!b_type) {
		static const GTypeInfo b_info = {
			sizeof(LINEOUTIFClass),
			NULL,	/* base_init */
			NULL,	/* base_finalise */
			NULL,   /* class_init */
			NULL,	/* class_finalize */
			NULL,	/* class_data */
			sizeof(LINEOUTIF),
			0,	/* n_preallocs */
			NULL    /* init */
		};

		b_type = g_type_register_static(MODULEWINDOW_TYPE,
						"LINEOUTIF", &b_info, 0);
	}
	return b_type;
}

static void lineout_if_destroy_cb(GtkWidget * widget, gpointer data)
{
	oss_dev_remove_writer(((oss_out *)data)->dev);
}

GtkWidget *lineout_if_new(oss_out * mod)
{
	LINEOUTIF *lineout_if;
	GtkWidget *menu, *menuitem, *optionmenu;
	GtkWidget *vbox, *hbox, *hbox2;
	GtkWidget *frame;
	GtkWidget *widget;

	lineout_if = gtk_type_new(lineout_if_get_type());

	modulewindow_set_module (MODULEWINDOW(lineout_if), (module *)mod);

	g_signal_connect(G_OBJECT(lineout_if), "destroy",
			 G_CALLBACK(lineout_if_destroy_cb), mod);

        /* No sliders, so allow automatic size */
        gtk_window_set_default_size (GTK_WINDOW(lineout_if), 0, 0);

        hbox2 = MODULEWINDOW(lineout_if)->headbox;

	/*
	   I N P U T 
	 */

	widget = inputoption_new((char *) "Input:", &mod->output_module, 0);
	gtk_box_pack_start(GTK_BOX(hbox2), widget, FALSE, FALSE, 4);
	gtk_widget_show(widget);

	frame = gtk_frame_new("Config");
	gtk_box_pack_start(GTK_BOX(MODULEWINDOW(lineout_if)->mainbox), frame, FALSE, TRUE, 0);
/*
   gtk_container_border_width(GTK_CONTAINER(frame), 4);
 */
	gtk_widget_show(frame);

	hbox = gtk_hbox_new(FALSE, 0);
	gtk_container_add(GTK_CONTAINER(frame), hbox);
	gtk_widget_show(hbox);

#if 0
	vbox = gtk_vbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(hbox), vbox, TRUE, FALSE, 0);
	gtk_widget_show(vbox);

	hbox3 = gtk_hbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), hbox3, TRUE, FALSE, 0);
	gtk_widget_show(hbox3);

	slider = slider_int_new("Vol", &mod->vol), 0, 64, 1);
	gtk_box_pack_start(GTK_BOX(hbox3), slider, TRUE, FALSE, 0);
	gtk_widget_show(slider);

	slider = slider_int_new("Pan", &mod->pan), 0, 32, 0);
	gtk_box_pack_start(GTK_BOX(vbox), slider, TRUE, FALSE, 0);
	gtk_widget_show(slider);
#endif

	/*
	   D E V I C E 
	 */

	frame = gtk_frame_new("Device");
	gtk_box_pack_start(GTK_BOX(hbox), frame, FALSE, FALSE, 0);
	gtk_container_border_width(GTK_CONTAINER(frame), 2);
	gtk_widget_show(frame);

	vbox = gtk_vbox_new(TRUE, 5);
	gtk_container_add(GTK_CONTAINER(frame), vbox);
	gtk_widget_show(vbox);


	/*
	   D E V I C E N A M E 
	 */

	optionmenu = gtk_option_menu_new();
	gtk_box_pack_start(GTK_BOX(vbox), optionmenu, FALSE, FALSE, 0);
	gtk_widget_show(optionmenu);
	menu = gtk_menu_new();

	menuitem = gtk_menu_item_new_with_label((char *) "/dev/dsp");
	gtk_menu_append(GTK_MENU(menu), menuitem);
	g_signal_connect(G_OBJECT(menuitem), "activate",
			 G_CALLBACK(lineout_if_set_device_dsp_cb),
			 mod);
	gtk_widget_show(menuitem);

	menuitem = gtk_menu_item_new_with_label((char *) "/dev/dsp1");
	gtk_menu_append(GTK_MENU(menu), menuitem);
	g_signal_connect(G_OBJECT(menuitem), "activate",
			 G_CALLBACK(lineout_if_set_device_dsp1_cb),
			 mod);
	gtk_widget_show(menuitem);

	gtk_option_menu_set_menu(GTK_OPTION_MENU(optionmenu), menu);


#if 0
	/*
	   C H A N N E L S 
	 */

	frame = gtk_frame_new("Channels");
	gtk_box_pack_start(GTK_BOX(hbox), frame, FALSE, FALSE, 0);
	gtk_container_border_width(GTK_CONTAINER(frame), 2);
	gtk_widget_show(frame);

	vbox = gtk_vbox_new(TRUE, 5);
	gtk_container_add(GTK_CONTAINER(frame), vbox);
	gtk_widget_show(vbox);

	widget = gtk_radio_button_new_with_label(NULL, "Mono");
	gtk_box_pack_start(GTK_BOX(vbox), widget, FALSE, FALSE, 0);
#if 0
	g_signal_connect(G_OBJECT(widget), "clicked",
			 G_CALLBACK(set_sinwave_cb), syre_if->data);
#endif
	gtk_widget_show(widget);

	rbgroup = gtk_radio_button_group(GTK_RADIO_BUTTON(widget));
	widget = gtk_radio_button_new_with_label(rbgroup, "Stereo");
	gtk_box_pack_start(GTK_BOX(vbox), widget, FALSE, FALSE, 0);
#if 0
	g_signal_connect(G_OBJECT(widget), "clicked",
			 G_CALLBACK(set_squarewave_cb), syre_if->data);
#endif
	gtk_widget_show(widget);


	/*
	   S A M P L E  S I Z E 
	 */

	frame = gtk_frame_new("Sample size");
	gtk_box_pack_start(GTK_BOX(hbox), frame, FALSE, FALSE, 0);
	gtk_container_border_width(GTK_CONTAINER(frame), 2);
	gtk_widget_show(frame);

	vbox = gtk_vbox_new(TRUE, 5);
	gtk_container_add(GTK_CONTAINER(frame), vbox);
	gtk_widget_show(vbox);

	widget = gtk_radio_button_new_with_label(NULL, "8 bit");
	gtk_box_pack_start(GTK_BOX(vbox), widget, FALSE, FALSE, 0);
#if 0
	g_signal_connect(G_OBJECT(widget), "clicked",
			 G_CALLBACK(set_sinwave_cb), syre_if->data);
#endif
	gtk_widget_show(widget);

	rbgroup = gtk_radio_button_group(GTK_RADIO_BUTTON(widget));
	widget = gtk_radio_button_new_with_label(rbgroup, "16 bit");
	gtk_box_pack_start(GTK_BOX(vbox), widget, FALSE, FALSE, 0);
#if 0
	g_signal_connect(G_OBJECT(widget), "clicked",
			 G_CALLBACK(set_squarewave_cb), syre_if->data);
#endif
	gtk_widget_show(widget);


	/*
	   P L A Y B A C K  F R E Q U E N C Y 
	 */

	frame = gtk_frame_new("Playback Frequency");
	gtk_box_pack_start(GTK_BOX(hbox), frame, FALSE, FALSE, 0);
	gtk_container_border_width(GTK_CONTAINER(frame), 2);
	gtk_widget_show(frame);

	vbox = gtk_vbox_new(TRUE, 5);
	gtk_container_add(GTK_CONTAINER(frame), vbox);
	gtk_widget_show(vbox);

	widget = gtk_radio_button_new_with_label(NULL, "44100 Hz");
	gtk_box_pack_start(GTK_BOX(vbox), widget, FALSE, FALSE, 0);
#if 0
	g_signal_connect(G_OBJECT(widget), "clicked",
			 G_CALLBACK(set_sinwave_cb), syre_if->data);
#endif
	gtk_widget_show(widget);

	rbgroup = gtk_radio_button_group(GTK_RADIO_BUTTON(widget));
	widget = gtk_radio_button_new_with_label(rbgroup, "22050 Hz");
	gtk_box_pack_start(GTK_BOX(vbox), widget, FALSE, FALSE, 0);
#if 0
	g_signal_connect(G_OBJECT(widget), "clicked",
			 G_CALLBACK(set_squarewave_cb), syre_if->data);
#endif
	gtk_widget_show(widget);

	rbgroup = gtk_radio_button_group(GTK_RADIO_BUTTON(widget));
	widget = gtk_radio_button_new_with_label(rbgroup, "11025 Hz");
	gtk_box_pack_start(GTK_BOX(vbox), widget, FALSE, FALSE, 0);
#if 0
	g_signal_connect(G_OBJECT(widget), "clicked",
			 G_CALLBACK(set_squarewave_cb), syre_if->data);
#endif
	gtk_widget_show(widget);

	rbgroup = gtk_radio_button_group(GTK_RADIO_BUTTON(widget));
	widget = gtk_radio_button_new_with_label(rbgroup, "5512 Hz");
	gtk_box_pack_start(GTK_BOX(vbox), widget, FALSE, FALSE, 0);
#if 0
	g_signal_connect(G_OBJECT(widget), "clicked",
			 G_CALLBACK(set_squarewave_cb), syre_if->data);
#endif
	gtk_widget_show(widget);
#endif

	return GTK_WIDGET(lineout_if);
}

void lineout_if_set_device_dsp_cb(GtkWidget * widget, gpointer data)
{
	lineout_set_devicename((oss_out *) data, (char *) "/dev/dsp");
}

void lineout_if_set_device_dsp1_cb(GtkWidget * widget, gpointer data)
{
	lineout_set_devicename((oss_out *) data, (char *) "/dev/dsp1");
}
