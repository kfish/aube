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
GtkWidget *xfader_if_new(xfader * xf);
void xfader_if_hide_cb(GtkWidget * widget, gpointer data);
void xfader_if_close_cb(GtkWidget * widget, gpointer data);
void xfader_if_onoff_cb(GtkWidget * widget, gpointer data);

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

GtkWidget *xfader_if_new(xfader * xf)
{
	XFaderIF *xfader_if;
	GtkWidget *vbox, *vbox2, *hbox, *hbox2;
	GtkWidget *frame;
	GtkWidget *button;
	GtkWidget *slider;

	xfader_if = gtk_type_new(xfader_if_get_type());

	xfader_if->data = xf;

	gtk_window_set_title(GTK_WINDOW(xfader_if),
			     ((module *) xfader_if->data)->u_label);

	gtk_container_border_width(GTK_CONTAINER(xfader_if), 1);

#if 0
	/*
	   when the window is given the "delete_event" signal - this is
	   * given by the window manager - usually the close option or on the
	   * titlebar - we ask it to call the delete_event() function
	   * as defined above. The data passed to the callback function is
	   * NULL and is ignored in the callback. 
	 */
	g_signal_connect(G_OBJECT(xfader_if), "delete_event",
			 G_CALLBACK(delete_event), NULL);
#endif

#if 1
	/*
	   here we connect the "destroy" event to a signal handler.
	   * This event occurs when we call gtk_widget_destroy() on the
	   * window, or if we return "TRUE" in the "delete_event" callback. 
	 */
	g_signal_connect(G_OBJECT(xfader_if), "destroy",
			 G_CALLBACK(xfader_if_close_cb), xfader_if);
#endif

	vbox2 = gtk_vbox_new(FALSE, 5);
	gtk_container_add(GTK_CONTAINER(xfader_if), vbox2);
	gtk_widget_show(vbox2);

	/*
	   I N P U T 
	 */

	hbox = gtk_hbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox2), hbox, FALSE, FALSE, 0);
	gtk_widget_show(hbox);

	button = gtk_toggle_button_new_with_label("On");
	gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 0);
	gtk_toggle_button_set_state(GTK_TOGGLE_BUTTON(button),
				    xfader_if->data->module.on);
	g_signal_connect(G_OBJECT(button), "clicked",
			 G_CALLBACK(xfader_if_onoff_cb), xfader_if->data);
	gtk_widget_show(button);

	button =
	    opsmenu_new((module *) xfader_if->data, GTK_WIDGET(xfader_if),
			xfader_if_hide_cb, xfader_if_close_cb);
	gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 4);
	gtk_widget_show(button);

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

void xfader_if_hide_cb(GtkWidget * widget, gpointer data)
{
	module *u;
	XFaderIF *xfader_if;

	xfader_if = XFADER_IF(data);
	u = (module *) xfader_if->data;
	aube_module_remove_if(u);

	gtk_widget_destroy(GTK_WIDGET(data));
}

void xfader_if_close_cb(GtkWidget * widget, gpointer data)
{
	module *u;
	XFaderIF *xfader_if;

	xfader_if = XFADER_IF(data);
	u = (module *) xfader_if->data;
	aube_remove_module(u);

	free((XFADER_IF(data))->data);
	gtk_widget_destroy(GTK_WIDGET(data));
}

void xfader_if_onoff_cb(GtkWidget * widget, gpointer data)
{
	aube_module_toggle((module *) data);
}
