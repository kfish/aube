#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <gtk/gtk.h>

#include "aube.h"
#include "master.h"
#include "opsmenu.h"
#include "outputlabel.h"
#include "atonal_if.h"
#include "slider.h"


extern atonal *atonal_new(void);

extern int tick;
extern char *note_names[97];

static void atonal_if_class_init(AtonalIFClass * klass);
static void atonal_if_init(AtonalIF * b);
GtkWidget *atonal_if_new(atonal * rsq);
void atonal_if_dismiss(GtkWidget * widget, gpointer data);

void atonal_if_update_at(GtkWidget * widget, gpointer data);
void atonal_if_hide_cb(GtkWidget * widget, gpointer data);
void atonal_if_close_cb(GtkWidget * widget, gpointer data);
void atonal_if_onoff_cb(GtkWidget * widget, gpointer data);
void atonal_if_set_note_cb(GtkWidget * widget, gpointer note);
void at_restart_cb(GtkWidget * widget, gpointer data);
void at_clear_cb(GtkWidget * widget, gpointer data);
void at_chaos_cb(GtkWidget * widget, gpointer data);
void at_play_once_cb(GtkWidget * widget, gpointer data);
void at_play_44_cb(GtkWidget * widget, gpointer data);
void at_transpose_u12_cb(GtkWidget * widget, gpointer data);
void at_transpose_d12_cb(GtkWidget * widget, gpointer data);
void at_transpose_u1_cb(GtkWidget * widget, gpointer data);
void at_transpose_d1_cb(GtkWidget * widget, gpointer data);
void at_lshift_cb(GtkWidget * widget, gpointer data);
void at_rshift_cb(GtkWidget * widget, gpointer data);

GType atonal_if_get_type(void)
{
	static GType b_type = 0;

	if (!b_type) {
		static const GTypeInfo b_info = {
			sizeof(AtonalIFClass),
			NULL,	/* base_init */
			NULL,	/* base_finalise */
			(GClassInitFunc) atonal_if_class_init,
			NULL,	/* class_finalize */
			NULL,	/* class_data */
			sizeof(AtonalIF),
			0,	/* n_preallocs */
			(GInstanceInitFunc) atonal_if_init,
		};

		b_type = g_type_register_static(GTK_TYPE_WINDOW,
						"AtonalIF", &b_info, 0);
	}
	return b_type;
}

static void atonal_if_class_init(AtonalIFClass * klass)
{

}

static void atonal_if_init(AtonalIF * atonal_if)
{
}

GtkWidget *atonal_if_new(atonal * rsq)
{
	AtonalIF *atonal_if;
	GtkWidget *vbox, *vbox2, *hbox, *hbox2;
	GtkWidget *button;
	GtkWidget *slider;
	gint i;
	gchar buf[4];

	atonal_if = gtk_type_new(atonal_if_get_type());

	atonal_if->data = rsq;

	gtk_window_set_title(GTK_WINDOW(atonal_if),
			     ((module *) atonal_if->data)->u_label);

	gtk_container_border_width(GTK_CONTAINER(atonal_if), 1);

#if 0
	/* when the window is given the "delete_event" signal - this is
	 * given by the window manager - usually the close option or on the
	 * titlebar - we ask it to call the delete_event() function
	 * as defined above. The data passed to the callback function is
	 * NULL and is ignored in the callback. */
	g_signal_connect(G_OBJECT(atonal_if), "delete_event",
			 G_CALLBACK(delete_event), NULL);
#endif

#if 1
	/* here we connect the "destroy" event to a signal handler.
	 * This event occurs when we call gtk_widget_destroy() on the
	 * window, or if we return "TRUE" in the "delete_event" callback. */
	g_signal_connect(G_OBJECT(atonal_if), "destroy",
			 G_CALLBACK(atonal_if_close_cb), atonal_if);
#endif

	vbox = gtk_vbox_new(FALSE, 5);
	gtk_container_add(GTK_CONTAINER(atonal_if), vbox);
	gtk_widget_show(vbox);

	hbox2 = gtk_hbox_new(FALSE, 5);
	gtk_box_pack_start(GTK_BOX(vbox), hbox2, FALSE, FALSE, 0);
	gtk_widget_show(hbox2);

	button = gtk_toggle_button_new_with_label("On");
	gtk_box_pack_start(GTK_BOX(hbox2), button, FALSE, FALSE, 1);
	gtk_toggle_button_set_state(GTK_TOGGLE_BUTTON(button),
				    atonal_if->data->module.on);
	g_signal_connect(G_OBJECT(button), "clicked",
			 G_CALLBACK(atonal_if_onoff_cb), atonal_if->data);
	gtk_widget_show(button);

	button =
	    opsmenu_new((module *) atonal_if->data, GTK_WIDGET(atonal_if),
			atonal_if_hide_cb, atonal_if_close_cb);
	gtk_box_pack_start(GTK_BOX(hbox2), button, FALSE, FALSE, 4);
	gtk_widget_show(button);

#if 0
	button = gtk_button_new_with_label("Clear");
	gtk_box_pack_start(GTK_BOX(hbox2), button, TRUE, TRUE, 1);
	g_signal_connect(G_OBJECT(button), "clicked",
			 G_CALLBACK(at_clear_cb), atonal_if);
	gtk_widget_show(button);

	button = gtk_button_new_with_label("Chaos");
	gtk_box_pack_start(GTK_BOX(hbox2), button, TRUE, TRUE, 1);
	g_signal_connect(G_OBJECT(button), "clicked",
			 G_CALLBACK(at_chaos_cb), atonal_if);
	gtk_widget_show(button);
#endif

	/* S E Q U E N C E */

	hbox = gtk_hbox_new(FALSE, 2);
	gtk_box_pack_start(GTK_BOX(vbox), hbox, TRUE, TRUE, 1);
	gtk_widget_show(hbox);

#if 0
	button = gtk_label_new("Triggers:");
	gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 3);
	gtk_widget_show(button);
#endif

#if 0
	slider = slider_int_new("", &(atonal_if->data->vol), 0, 64, 0);
	gtk_box_pack_start(GTK_BOX(hbox), slider, FALSE, FALSE, 0);
	gtk_widget_show(slider);
#endif

	for (i = 0; i < AT_LENGTH; i++) {

		if (i && i % 4 == 0) {
			button = gtk_vseparator_new();
			gtk_box_pack_start(GTK_BOX(hbox), button, FALSE,
					   FALSE, 3);
			gtk_widget_show(button);
		}

		vbox2 = gtk_vbox_new(FALSE, 1);
		gtk_box_pack_start(GTK_BOX(hbox), vbox2, TRUE, TRUE, 0);
		gtk_widget_show(vbox2);

		snprintf(buf, 4, "%d", i);
		slider =
		    slider_int_new(buf,
				   &(atonal_if->data->sequence[i].note),
				   110, 660, 1);
		gtk_box_pack_start(GTK_BOX(vbox2), slider, TRUE, TRUE, 0);
		gtk_widget_show(slider);

		button = gtk_toggle_button_new();
		gtk_box_pack_start(GTK_BOX(vbox2), button, FALSE, FALSE,
				   1);
		gtk_widget_set_usize(button, 10, 10);
		gtk_toggle_button_set_state(GTK_TOGGLE_BUTTON(button),
					    (atonal_if->data->sequence[i].
					     trigger == 1));
		g_signal_connect(G_OBJECT(button), "clicked",
				 G_CALLBACK(atonal_if_set_note_cb),
				 atonal_if);
		gtk_widget_show(button);
		atonal_if->buttons[i].button = button;
	}

/*
    button = outputlabel_new((module *)atonal_if->data, j);
    gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 1);
    gtk_widget_show(button);
*/


	return GTK_WIDGET(atonal_if);
}

void atonal_if_update_at(GtkWidget * widget, gpointer data)
{
	AtonalIF *atonal_if;
	int i;

	atonal_if = ATONAL_IF(data);
	for (i = 0; i < AT_LENGTH; i++) {
		g_signal_handlers_block_matched(atonal_if->buttons[i].
						button,
						G_SIGNAL_MATCH_DATA, 0, 0,
						NULL, NULL, atonal_if);
		gtk_toggle_button_set_state(GTK_TOGGLE_BUTTON
					    (atonal_if->buttons[i].button),
					    (atonal_if->data->sequence[i].
					     trigger == 1));
		g_signal_handlers_unblock_matched(atonal_if->buttons[i].
						  button,
						  G_SIGNAL_MATCH_DATA, 0,
						  0, NULL, NULL,
						  atonal_if);
	}
}

void atonal_if_hide_cb(GtkWidget * widget, gpointer data)
{
	module *u;
	AtonalIF *atonal_if;

	atonal_if = ATONAL_IF(data);
	u = (module *) atonal_if->data;
	aube_module_remove_if(u);

	gtk_widget_destroy(GTK_WIDGET(data));
}

void atonal_if_close_cb(GtkWidget * widget, gpointer data)
{
	module *u;
	AtonalIF *atonal_if;

	atonal_if = ATONAL_IF(data);

	u = (module *) atonal_if->data;
	aube_remove_module(u);

	free((ATONAL_IF(data))->data);
	gtk_widget_destroy(GTK_WIDGET(data));
}

void atonal_if_dismiss(GtkWidget * widget, gpointer data)
{
	gtk_widget_destroy(GTK_WIDGET(data));
}

void atonal_if_onoff_cb(GtkWidget * widget, gpointer data)
{
	aube_module_toggle((module *) data);
}

void atonal_if_set_note_cb(GtkWidget * widget, gpointer data)
{
	AtonalIF *atonal_if;
	int i;

	atonal_if = ATONAL_IF(data);
	for (i = 0; i < AT_LENGTH; i++) {
		if (atonal_if->buttons[i].button == widget) {
			atonal_if->data->sequence[i].trigger = 1 -
			    atonal_if->data->sequence[i].trigger;
			break;
		}
	}

	atonal_if_update_at(NULL, atonal_if);
}

void at_restart_cb(GtkWidget * widget, gpointer data)
{
	at_restart((atonal *) data);
}

void at_clear_cb(GtkWidget * widget, gpointer data)
{
	AtonalIF *atonal_if;

	atonal_if = ATONAL_IF(data);
	at_clear(atonal_if->data);
	atonal_if_update_at(NULL, atonal_if);
}

void at_chaos_cb(GtkWidget * widget, gpointer data)
{
	AtonalIF *atonal_if;

	atonal_if = ATONAL_IF(data);
	at_chaos(atonal_if->data);
	atonal_if_update_at(NULL, atonal_if);
}

void at_play_once_cb(GtkWidget * widget, gpointer data)
{
	AtonalIF *atonal_if;

	atonal_if = ATONAL_IF(data);
	at_play_once(atonal_if->data);
	atonal_if_update_at(NULL, atonal_if);
}

void at_play_44_cb(GtkWidget * widget, gpointer data)
{
	AtonalIF *atonal_if;

	atonal_if = ATONAL_IF(data);
	at_play_44(atonal_if->data);
	atonal_if_update_at(NULL, atonal_if);
}

void at_transpose_u12_cb(GtkWidget * widget, gpointer data)
{
	AtonalIF *atonal_if;

	atonal_if = ATONAL_IF(data);
	at_transpose_u12(atonal_if->data);
	atonal_if_update_at(NULL, atonal_if);
}

void at_transpose_d12_cb(GtkWidget * widget, gpointer data)
{
	AtonalIF *atonal_if;

	atonal_if = ATONAL_IF(data);
	at_transpose_d12(atonal_if->data);
	atonal_if_update_at(NULL, atonal_if);
}

void at_transpose_u1_cb(GtkWidget * widget, gpointer data)
{
	AtonalIF *atonal_if;

	atonal_if = ATONAL_IF(data);
	at_transpose_u1(atonal_if->data);
	atonal_if_update_at(NULL, atonal_if);
}

void at_transpose_d1_cb(GtkWidget * widget, gpointer data)
{
	AtonalIF *atonal_if;

	atonal_if = ATONAL_IF(data);
	at_transpose_d1(atonal_if->data);
	atonal_if_update_at(NULL, atonal_if);
}

void at_lshift_cb(GtkWidget * widget, gpointer data)
{
	at_shift_left((atonal *) data);
}

void at_rshift_cb(GtkWidget * widget, gpointer data)
{
	at_shift_right((atonal *) data);
}
