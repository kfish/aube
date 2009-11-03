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

#include "linein_if.h"
#include "aube.h"
#include "slider.h"
#include "opsmenu.h"
#include "outputlabel.h"

static void linein_if_class_init(LINEINIFClass * klass);
static void linein_if_init(LINEINIF * b);
GtkWidget *linein_if_new(oss_in * oss_p);
void linein_if_hide_cb(GtkWidget * widget, gpointer data);
void linein_if_close_cb(GtkWidget * widget, gpointer data);
void linein_if_onoff_cb(GtkWidget * widget, gpointer data);
void linein_if_set_device_dsp_cb(GtkWidget * widget, gpointer data);
void linein_if_set_device_dsp1_cb(GtkWidget * widget, gpointer data);

#if 0
void linein_if_set_mode_output_cb(GtkWidget * widget, gpointer data);
void linein_if_set_mode_input_cb(GtkWidget * widget, gpointer data);
void linein_if_set_mode_duplex_cb(GtkWidget * widget, gpointer data);
#endif

GType linein_if_get_type(void)
{
	static GType b_type = 0;

	if (!b_type) {
		static const GTypeInfo b_info = {
			sizeof(LINEINIFClass),
			NULL,	/* base_init */
			NULL,	/* base_finalise */
			(GClassInitFunc) linein_if_class_init,
			NULL,	/* class_finalize */
			NULL,	/* class_data */
			sizeof(LINEINIF),
			0,	/* n_preallocs */
			(GInstanceInitFunc) linein_if_init,
		};

		b_type = g_type_register_static(MODULEWINDOW_TYPE,
						"LINEINIF", &b_info, 0);
	}
	return b_type;
}

static void linein_if_class_init(LINEINIFClass * klass)
{

}

static void linein_if_init(LINEINIF * linein_if)
{
}

GtkWidget *linein_if_new(oss_in * oss_p)
{
	LINEINIF *linein_if;
	GtkWidget *menu, *menuitem, *optionmenu;
	GtkWidget *vbox, *vbox2, *hbox, *hbox2;
	GtkWidget *frame;
	GtkWidget *widget;

	linein_if = gtk_type_new(linein_if_get_type());

	modulewindow_set_module (MODULEWINDOW(linein_if), (module *)oss_p);

	linein_if->data = oss_p;

#if 0
	g_signal_connect(G_OBJECT(linein_if), "delete_event",
			 G_CALLBACK(delete_event), NULL);
#endif

#if 1
	g_signal_connect(G_OBJECT(linein_if), "destroy",
			 G_CALLBACK(linein_if_close_cb), linein_if);
#endif

	vbox2 = gtk_vbox_new(FALSE, 5);
	gtk_container_add(GTK_CONTAINER(linein_if), vbox2);
	gtk_widget_show(vbox2);

#if 0
	/*
	   M E N U B A R 
	 */

	menubar = gtk_menu_bar_new();
	gtk_box_pack_start(GTK_BOX(vbox2), menubar, FALSE, TRUE, 0);

	/*
	   FILE MENU 
	 */

	menu = gtk_menu_new();

	menuitem = gtk_menu_item_new_with_label("Close");
	gtk_menu_append(GTK_MENU(menu), menuitem);
	g_signal_connect(G_OBJECT(menuitem), "activate",
			 G_CALLBACK(linein_if_close_cb), linein_if);
	gtk_widget_show(menuitem);

	menuitem = gtk_menu_item_new_with_label("File");
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(menuitem), menu);
	gtk_menu_bar_append(GTK_MENU_BAR(menubar), menuitem);
	gtk_widget_show(menuitem);

	gtk_widget_show(menubar);
#endif

	hbox2 = gtk_hbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox2), hbox2, FALSE, FALSE, 0);
	gtk_widget_show(hbox2);

	widget = gtk_toggle_button_new_with_label("On");
	gtk_box_pack_start(GTK_BOX(hbox2), widget, FALSE, FALSE, 1);
	gtk_toggle_button_set_state(GTK_TOGGLE_BUTTON(widget),
				    linein_if->data->input_module.on);
	g_signal_connect(G_OBJECT(widget), "clicked",
			 G_CALLBACK(linein_if_onoff_cb), linein_if->data);
	gtk_widget_show(widget);

	widget =
	    opsmenu_new(&linein_if->data->input_module,
			GTK_WIDGET(linein_if), linein_if_hide_cb,
			linein_if_close_cb);
	gtk_box_pack_start(GTK_BOX(hbox2), widget, FALSE, FALSE, 2);
	gtk_widget_show(widget);


	/*
	   O U T P U T 
	 */

	widget = outputlabel_new(&linein_if->data->input_module, 0);
	gtk_box_pack_start(GTK_BOX(hbox2), widget, FALSE, FALSE, 4);
	gtk_widget_show(widget);

	frame = gtk_frame_new("Config");
	gtk_box_pack_start(GTK_BOX(vbox2), frame, FALSE, TRUE, 0);
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

	slider = slider_int_new("Vol", &(linein_if->data->vol), 0, 64, 1);
	gtk_box_pack_start(GTK_BOX(hbox3), slider, TRUE, FALSE, 0);
	gtk_widget_show(slider);

	slider = slider_int_new("Pan", &(linein_if->data->pan), 0, 32, 0);
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


#if 0
	/*
	   M O D E 
	 */

	optionmenu = gtk_option_menu_new();
	gtk_box_pack_start(GTK_BOX(vbox), optionmenu, FALSE, FALSE, 0);
	gtk_widget_show(optionmenu);
	menu = gtk_menu_new();

	menuitem = gtk_menu_item_new_with_label((char *) "Output");
	gtk_menu_append(GTK_MENU(menu), menuitem);
	g_signal_connect(G_OBJECT(menuitem), "activate",
			 G_CALLBACK(linein_if_set_mode_output_cb),
			 linein_if->data);
	gtk_widget_show(menuitem);

	menuitem = gtk_menu_item_new_with_label((char *) "Input");
	gtk_menu_append(GTK_MENU(menu), menuitem);
	g_signal_connect(G_OBJECT(menuitem), "activate",
			 G_CALLBACK(linein_if_set_mode_input_cb),
			 linein_if->data);
	gtk_widget_show(menuitem);

	menuitem = gtk_menu_item_new_with_label((char *) "Duplex");
	gtk_menu_append(GTK_MENU(menu), menuitem);
	g_signal_connect(G_OBJECT(menuitem), "activate",
			 G_CALLBACK(linein_if_set_mode_duplex_cb),
			 linein_if->data);
	gtk_widget_show(menuitem);

	gtk_option_menu_set_menu(GTK_OPTION_MENU(optionmenu), menu);
#endif


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
			 G_CALLBACK(linein_if_set_device_dsp_cb),
			 linein_if->data);
	gtk_widget_show(menuitem);

	menuitem = gtk_menu_item_new_with_label((char *) "/dev/dsp1");
	gtk_menu_append(GTK_MENU(menu), menuitem);
	g_signal_connect(G_OBJECT(menuitem), "activate",
			 G_CALLBACK(linein_if_set_device_dsp1_cb),
			 linein_if->data);
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

	return GTK_WIDGET(linein_if);
}

void linein_if_hide_cb(GtkWidget * widget, gpointer data)
{
	LINEINIF *linein_if;

	linein_if = LINEIN_IF(data);
	aube_module_remove_if(&linein_if->data->input_module);

	gtk_widget_destroy(GTK_WIDGET(data));
}

void linein_if_close_cb(GtkWidget * widget, gpointer data)
{
	LINEINIF *linein_if;

	linein_if = LINEIN_IF(data);
	oss_dev_remove_writer(linein_if->data->dev);

	aube_remove_module(&linein_if->data->input_module);

	free(linein_if->data);
	gtk_widget_destroy(GTK_WIDGET(data));
}

void linein_if_onoff_cb(GtkWidget * widget, gpointer data)
{
	aube_module_toggle(&((oss_in *) data)->input_module);
	if (((oss *) data)->output_module.on) {
		oss_open(((oss_in *) data)->dev);
	} else {
		oss_close(((oss_in *) data)->dev);
	}
}

void linein_if_set_device_dsp_cb(GtkWidget * widget, gpointer data)
{
	linein_set_devicename((oss_in *) data, (char *) "/dev/dsp");
}

void linein_if_set_device_dsp1_cb(GtkWidget * widget, gpointer data)
{
	linein_set_devicename((oss_in *) data, (char *) "/dev/dsp1");
}

/*
   OBSOLETE 
 */
#if 0
void linein_if_set_mode_output_cb(GtkWidget * widget, gpointer data)
{
	oss_set_mode((oss_in *) data, OSS_MODE_OUTPUT);
}

void linein_if_set_mode_input_cb(GtkWidget * widget, gpointer data)
{
	oss_set_mode((oss_in *) data, OSS_MODE_INPUT);
}

void linein_if_set_mode_duplex_cb(GtkWidget * widget, gpointer data)
{
	oss_set_mode((oss_in *) data, OSS_MODE_DUPLEX);
}

#endif
