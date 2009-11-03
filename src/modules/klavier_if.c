#include <stdlib.h>
#include <strings.h>
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

#include "aube.h"
#include "master.h"
#include "opsmenu.h"
#include "outputlabel.h"
#include "klavier_if.h"
#include "slider.h"
#include "klav.h"

extern klavier *klavier_new(void);

extern int tick;

static void klavier_if_class_init(KlavierIFClass * klass);
static void klavier_if_init(KlavierIF * b);
GtkWidget *klavier_if_new(klavier * k);

void klavier_if_hide_cb(GtkWidget * widget, gpointer data);
void klavier_if_close_cb(GtkWidget * widget, gpointer data);
void klavier_if_onoff_cb(GtkWidget * widget, gpointer data);
void klavkey_press_event(GtkWidget * widget, gint key, gpointer data);
void klavkey_release_event(GtkWidget * widget, gint key, gpointer data);
void klavkey_key_press_event(GtkWidget * widget, GdkEventKey * event,
			     gpointer data);
void klavkey_key_release_event(GtkWidget * widget, GdkEventKey * event,
			       gpointer data);
gint get_note_from_key(guint keyval);

GType klavier_if_get_type(void)
{
	static GType b_type = 0;

	if (!b_type) {
		static const GTypeInfo b_info = {
			sizeof(KlavierIFClass),
			NULL,	/* base_init */
			NULL,	/* base_finalise */
			(GClassInitFunc) klavier_if_class_init,
			NULL,	/* class_finalize */
			NULL,	/* class_data */
			sizeof(KlavierIF),
			0,	/* n_preallocs */
			(GInstanceInitFunc) klavier_if_init,
		};

		b_type = g_type_register_static(MODULEWINDOW_TYPE,
						"KlavierIF", &b_info, 0);
	}
	return b_type;
}

static void klavier_if_class_init(KlavierIFClass * klass)
{

}

static void klavier_if_init(KlavierIF * klavier_if)
{
}

GtkWidget *klavier_if_new(klavier * k)
{
	KlavierIF *klavier_if;
	GtkWidget *vbox, *vbox2, *hbox, *hbox2, *hbox3;
	GtkWidget *ebox;
	GtkWidget *frame;
	GtkWidget *button;
	GtkWidget *klav;
	GtkWidget *slider;

	gint start, end, keywidth;
	KlavType klavtype = KLAV_TYPE_NORMAL;
	KlavDir klavdir = KLAV_DIR_HORIZONTAL;

	klavier_if = gtk_type_new(klavier_if_get_type());

	klavier_if->data = k;

	gtk_window_set_title(GTK_WINDOW(klavier_if),
			     ((module *) klavier_if->data)->u_label);

	gtk_container_border_width(GTK_CONTAINER(klavier_if), 1);

#if 0
	/*
	   when the window is given the "delete_event" signal - this is
	   * given by the window manager - usually the close option or on the
	   * titlebar - we ask it to call the delete_event() function
	   * as defined above. The data passed to the callback function is
	   * NULL and is ignored in the callback. 
	 */
	g_signal_connect(G_OBJECT(klavier_if), "delete_event",
			 G_CALLBACK(delete_event), NULL);

#endif

#if 1
	/*
	   here we connect the "destroy" event to a signal handler.
	   * This event occurs when we call gtk_widget_destroy() on the
	   * window, or if we return "TRUE" in the "delete_event" callback. 
	 */
	g_signal_connect(G_OBJECT(klavier_if), "destroy",
			 G_CALLBACK(klavier_if_close_cb), klavier_if);
#endif

	vbox2 = gtk_vbox_new(FALSE, 5);
	gtk_container_add(GTK_CONTAINER(klavier_if), vbox2);
	gtk_widget_show(vbox2);

	hbox2 = gtk_hbox_new(FALSE, 5);
	gtk_box_pack_start(GTK_BOX(vbox2), hbox2, TRUE, TRUE, 0);
	gtk_widget_show(hbox2);

	button = gtk_toggle_button_new_with_label("On");
	gtk_box_pack_start(GTK_BOX(hbox2), button, FALSE, FALSE, 1);
	gtk_toggle_button_set_state(GTK_TOGGLE_BUTTON(button),
				    klavier_if->data->module.on);
	g_signal_connect(G_OBJECT(button), "clicked",
			 G_CALLBACK(klavier_if_onoff_cb),
			 klavier_if->data);
	gtk_widget_show(button);

	button =
	    opsmenu_new((module *) klavier_if->data,
			GTK_WIDGET(klavier_if), klavier_if_hide_cb,
			klavier_if_close_cb);
	gtk_box_pack_start(GTK_BOX(hbox2), button, FALSE, FALSE, 4);
	gtk_widget_show(button);

	button =
	    outputlabel_new((module *) KLAVIER_IF(klavier_if)->data, 0);
	gtk_box_pack_start(GTK_BOX(hbox2), button, FALSE, FALSE, 4);
	gtk_widget_show(button);

	hbox2 = gtk_hbox_new(FALSE, 5);
	gtk_box_pack_start(GTK_BOX(vbox2), hbox2, TRUE, TRUE, 0);
	gtk_widget_show(hbox2);

	/*
	   K L A V I E R 
	 */

	frame = gtk_frame_new(NULL);
	gtk_box_pack_start(GTK_BOX(hbox2), frame, FALSE, TRUE, 2);
/*
   gtk_container_border_width(GTK_CONTAINER(frame), 4);
 */
	gtk_widget_show(frame);

	hbox = gtk_hbox_new(FALSE, 0);
	gtk_container_add(GTK_CONTAINER(frame), hbox);
	gtk_widget_show(hbox);

	vbox = gtk_vbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(hbox), vbox, TRUE, FALSE, 0);
	gtk_widget_show(vbox);

	hbox3 = gtk_hbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), hbox3, TRUE, TRUE, 0);
	gtk_widget_show(hbox3);

	slider = slider_int_new("Vol", &(klavier_if->data->vol), 0, 64, 1);
	gtk_box_pack_start(GTK_BOX(hbox3), slider, TRUE, FALSE, 0);
	gtk_widget_show(slider);

	slider =
	    slider_int_new("Tune", &(klavier_if->data->tune), 1, 1024, 1);
	gtk_box_pack_start(GTK_BOX(hbox3), slider, TRUE, FALSE, 0);
	gtk_widget_show(slider);

	slider = slider_int_new("Pan", &(klavier_if->data->pan), 0, 32, 0);
	gtk_box_pack_start(GTK_BOX(vbox), slider, FALSE, FALSE, 0);
	gtk_widget_show(slider);


	/*
	   K L A V I E R 
	 */

	frame = gtk_frame_new(NULL);
	gtk_frame_set_shadow_type(GTK_FRAME(frame), GTK_SHADOW_OUT);
	gtk_box_pack_start(GTK_BOX(hbox2), frame, TRUE, TRUE, 0);
	gtk_container_border_width(GTK_CONTAINER(frame), 0);
	gtk_widget_show(frame);

	hbox = gtk_hbox_new(FALSE, 0);
	gtk_container_add(GTK_CONTAINER(frame), hbox);
	gtk_widget_show(hbox);

	ebox = gtk_event_box_new();
	gtk_box_pack_start(GTK_BOX(hbox), ebox, FALSE, FALSE, 0);
	gtk_widget_show(ebox);

	klav = klav_new();
	start = 36;
	end = 95;
	if (klavtype == KLAV_TYPE_NORMAL)
		keywidth = 8;
	else
		keywidth = 10;

	klav_set_range(KLAV(klav), start, end);

	if (klavdir == KLAV_DIR_VERTICAL) {
		gtk_drawing_area_size(GTK_DRAWING_AREA(klav), 60,
				      (end - start + 1) * keywidth);
		klav_set_klav_dir(KLAV(klav), KLAV_DIR_VERTICAL);
	} else {
		gtk_drawing_area_size(GTK_DRAWING_AREA(klav),
				      (end - start + 1) * keywidth, 60);
		klav_set_klav_dir(KLAV(klav), KLAV_DIR_HORIZONTAL);
	}

	klav_set_klav_type(KLAV(klav), klavtype);

	g_signal_connect(G_OBJECT(klav), "klavkey_press",
			 G_CALLBACK(klavkey_press_event), klavier_if);

	g_signal_connect(G_OBJECT(klav), "klavkey_release",
			 G_CALLBACK(klavkey_release_event), klavier_if);

	g_signal_connect(G_OBJECT(ebox), "key_press_event",
			 G_CALLBACK(klavkey_key_press_event), klavier_if);

	g_signal_connect(G_OBJECT(ebox), "key_release_event",
			 G_CALLBACK(klavkey_key_release_event),
			 klavier_if);

	/*
	   gtk_box_pack_start(GTK_BOX(hbox), klav, FALSE, FALSE, 0);
	 */
	gtk_container_add(GTK_CONTAINER(ebox), klav);
	gtk_widget_show(klav);

	return GTK_WIDGET(klavier_if);
}

void klavier_if_hide_cb(GtkWidget * widget, gpointer data)
{
	module *u;
	KlavierIF *klavier_if;

	klavier_if = KLAVIER_IF(data);

	u = (module *) klavier_if->data;
	aube_module_remove_if(u);

	gtk_widget_destroy(GTK_WIDGET(data));
}

void klavier_if_close_cb(GtkWidget * widget, gpointer data)
{
	module *u;
	KlavierIF *klavier_if;

	klavier_if = KLAVIER_IF(data);

	u = (module *) klavier_if->data;
	aube_remove_module(u);

	free((KLAVIER_IF(data))->data);
	gtk_widget_destroy(GTK_WIDGET(data));
}

void klavier_if_onoff_cb(GtkWidget * widget, gpointer data)
{
	aube_module_toggle((module *) data);
}

void klavkey_press_event(GtkWidget * widget, gint key, gpointer data)
{
	KLAVIER_IF(data)->data->last_klav_key = key;
	klav_press(KLAV(widget), key);
}

void klavkey_release_event(GtkWidget * widget, gint key, gpointer data)
{
	klav_release(KLAV(widget), key);
}

void
klavkey_key_press_event(GtkWidget * widget, GdkEventKey * event,
			gpointer data)
{
	gint key = get_note_from_key(event->keyval);

	g_print(".");

	if (key >= 0) {
		KLAVIER_IF(data)->data->last_klav_key = key;
		klav_press(KLAV(KLAVIER_IF(data)->data), key);
	}
}

void
klavkey_key_release_event(GtkWidget * widget, GdkEventKey * event,
			  gpointer data)
{
	gint key = get_note_from_key(event->keyval);

	if (key >= 0) {
		klav_release(KLAV(widget), key);
		klav_release(KLAV(KLAVIER_IF(data)->data), key);
	}
}

gint get_note_from_key(guint keyval)
{
	switch (keyval) {
	case GDK_z:
	case GDK_Z:
		return 0;
		break;
	case GDK_s:
	case GDK_S:
		return 1;
		break;
	case GDK_x:
	case GDK_X:
		return 2;
		break;
	case GDK_d:
	case GDK_D:
		return 3;
		break;
	case GDK_c:
	case GDK_C:
		return 4;
		break;
	case GDK_v:
	case GDK_V:
		return 5;
		break;
	case GDK_g:
	case GDK_G:
		return 6;
		break;
	case GDK_b:
	case GDK_B:
		return 7;
		break;
	case GDK_h:
	case GDK_H:
		return 8;
		break;
	case GDK_n:
	case GDK_N:
		return 9;
		break;
	case GDK_j:
	case GDK_J:
		return 10;
		break;
	case GDK_m:
	case GDK_M:
		return 11;
		break;
	case GDK_comma:
	case GDK_less:
	case GDK_q:
	case GDK_Q:
		return 12;
		break;
	case GDK_l:
	case GDK_L:
	case GDK_2:
	case GDK_at:
		return 13;
		break;
	case GDK_period:
	case GDK_greater:
	case GDK_w:
	case GDK_W:
		return 14;
		break;
	case GDK_semicolon:
	case GDK_colon:
	case GDK_3:
	case GDK_numbersign:
		return 15;
		break;
	case GDK_slash:
	case GDK_question:
	case GDK_e:
	case GDK_E:
		return 16;
		break;
	case GDK_r:
	case GDK_R:
		return 17;
		break;
	case GDK_5:
	case GDK_percent:
		return 18;
		break;
	case GDK_t:
	case GDK_T:
		return 19;
		break;
	case GDK_6:
	case GDK_asciicircum:	/*
				   ????? 
				 */
		return 20;
		break;
	case GDK_y:
	case GDK_Y:
		return 21;
		break;
	case GDK_7:
	case GDK_ampersand:
		return 22;
		break;
	case GDK_u:
	case GDK_U:
		return 23;
		break;
	case GDK_i:
	case GDK_I:
		return 24;
		break;
	case GDK_9:
	case GDK_parenleft:
		return 25;
		break;
	case GDK_o:
	case GDK_O:
		return 26;
		break;
	case GDK_0:
	case GDK_parenright:
		return 27;
		break;
	case GDK_p:
	case GDK_P:
		return 28;
		break;
	case GDK_bracketleft:
	case GDK_braceleft:
		return 29;
		break;
	case GDK_equal:
	case GDK_plus:
		return 30;
		break;
	case GDK_bracketright:
	case GDK_braceright:
		return 31;
		break;
	default:
		return -1;
		break;
	}
}
