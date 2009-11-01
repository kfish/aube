#include <stdlib.h>
#include <strings.h>
#include <gtk/gtk.h>

#include "lineout_if.h"
#include "aube.h"
#include "slider.h"
#include "inputoption.h"
#include "opsmenu.h"

static void lineout_if_class_init(LINEOUTIFClass * klass);
static void lineout_if_init(LINEOUTIF * b);
GtkWidget *lineout_if_new(oss_out * oss_p);
void lineout_if_dismiss(GtkWidget * widget, gpointer data);
void lineout_if_hide_cb(GtkWidget * widget, gpointer data);
void lineout_if_close_cb(GtkWidget * widget, gpointer data);
void lineout_if_onoff_cb(GtkWidget * widget, gpointer data);
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
			(GClassInitFunc) lineout_if_class_init,
			NULL,	/* class_finalize */
			NULL,	/* class_data */
			sizeof(LINEOUTIF),
			0,	/* n_preallocs */
			(GInstanceInitFunc) lineout_if_init,
		};

		b_type = g_type_register_static(MODULEWINDOW_TYPE,
						"LINEOUTIF", &b_info, 0);
	}
	return b_type;
}

static void lineout_if_class_init(LINEOUTIFClass * klass)
{

}

static void lineout_if_init(LINEOUTIF * lineout_if)
{
}

GtkWidget *lineout_if_new(oss_out * oss_p)
{
	LINEOUTIF *lineout_if;
	GtkWidget *menu, *menuitem, *optionmenu;
	GtkWidget *vbox, *vbox2, *hbox, *hbox2;
	GtkWidget *frame;
	GtkWidget *widget;

	lineout_if = gtk_type_new(lineout_if_get_type());

	lineout_if->data = oss_p;

	gtk_window_set_title(GTK_WINDOW(lineout_if),
			     lineout_if->data->output_module.u_label);

	gtk_container_border_width(GTK_CONTAINER(lineout_if), 1);

#if 0
	g_signal_connect(G_OBJECT(lineout_if), "delete_event",
			 G_CALLBACK(delete_event), NULL);
#endif

#if 1
	g_signal_connect(G_OBJECT(lineout_if), "destroy",
			 G_CALLBACK(lineout_if_close_cb), lineout_if);
#endif

	vbox2 = gtk_vbox_new(FALSE, 5);
	gtk_container_add(GTK_CONTAINER(lineout_if), vbox2);
	gtk_widget_show(vbox2);

	hbox2 = gtk_hbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox2), hbox2, FALSE, FALSE, 0);
	gtk_widget_show(hbox2);

	widget = gtk_toggle_button_new_with_label("On");
	gtk_box_pack_start(GTK_BOX(hbox2), widget, FALSE, FALSE, 1);
	gtk_toggle_button_set_state(GTK_TOGGLE_BUTTON(widget),
				    lineout_if->data->output_module.on);
	g_signal_connect(G_OBJECT(widget), "clicked",
			 G_CALLBACK(lineout_if_onoff_cb),
			 lineout_if->data);
	gtk_widget_show(widget);

	widget =
	    opsmenu_new(&lineout_if->data->output_module,
			GTK_WIDGET(lineout_if), lineout_if_hide_cb,
			lineout_if_close_cb);
	gtk_box_pack_start(GTK_BOX(hbox2), widget, FALSE, FALSE, 2);
	gtk_widget_show(widget);


	/*
	   I N P U T 
	 */

	widget =
	    inputoption_new((char *) "Input:",
			    &lineout_if->data->output_module, 0);
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

	slider = slider_int_new("Vol", &(lineout_if->data->vol), 0, 64, 1);
	gtk_box_pack_start(GTK_BOX(hbox3), slider, TRUE, FALSE, 0);
	gtk_widget_show(slider);

	slider = slider_int_new("Pan", &(lineout_if->data->pan), 0, 32, 0);
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
			 lineout_if->data);
	gtk_widget_show(menuitem);

	menuitem = gtk_menu_item_new_with_label((char *) "/dev/dsp1");
	gtk_menu_append(GTK_MENU(menu), menuitem);
	g_signal_connect(G_OBJECT(menuitem), "activate",
			 G_CALLBACK(lineout_if_set_device_dsp1_cb),
			 lineout_if->data);
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

void lineout_if_hide_cb(GtkWidget * widget, gpointer data)
{
	LINEOUTIF *lineout_if;

	lineout_if = LINEOUT_IF(data);
	aube_module_remove_if(&lineout_if->data->output_module);

	gtk_widget_destroy(GTK_WIDGET(data));
}

void lineout_if_close_cb(GtkWidget * widget, gpointer data)
{
	LINEOUTIF *lineout_if;

	lineout_if = LINEOUT_IF(data);
	oss_dev_remove_writer(lineout_if->data->dev);

	aube_remove_module(&lineout_if->data->output_module);
	/*
	   aube_remove_module(&lineout_if->data->input_module);
	 */

	free(lineout_if->data);
	gtk_widget_destroy(GTK_WIDGET(data));
}

void lineout_if_dismiss(GtkWidget * widget, gpointer data)
{
	gtk_widget_destroy(GTK_WIDGET(data));
}

void lineout_if_onoff_cb(GtkWidget * widget, gpointer data)
{
	aube_module_toggle(&((oss_out *) data)->output_module);
}

void lineout_if_set_device_dsp_cb(GtkWidget * widget, gpointer data)
{
	lineout_set_devicename((oss_out *) data, (char *) "/dev/dsp");
}

void lineout_if_set_device_dsp1_cb(GtkWidget * widget, gpointer data)
{
	lineout_set_devicename((oss_out *) data, (char *) "/dev/dsp1");
}
