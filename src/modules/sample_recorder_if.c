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
#include <gdk/gdkkeysyms.h>

#include "sample_recorder_if.h"
#include "aube.h"
#include "master.h"
#include "slider.h"
#include "inputoption.h"
#include "outputlabel.h"

extern int tick;

static void sample_recorder_if_class_init(SampleRecorderIFClass * klass);
static void sample_recorder_if_init(SampleRecorderIF * b);
GtkWidget *sample_recorder_if_new(sample_recorder * mod);
void sample_recorder_if_record(GtkWidget * widget, gpointer data);
void sample_recorder_if_stop(GtkWidget * widget, gpointer data);
void sample_recorder_if_record_next(GtkWidget * widget, gpointer data);
void sample_recorder_if_set_record_states(SampleRecorderIF *
					  sample_recorder_if);
void sample_recorder_if_passthrough_never(GtkWidget * widget,
					  gpointer data);
void sample_recorder_if_passthrough_rec(GtkWidget * widget, gpointer data);
void sample_recorder_if_passthrough_always(GtkWidget * widget,
					   gpointer data);
void sample_recorder_if_restart(GtkWidget * widget, gpointer data);
void sample_recorder_if_check_record(SampleRecorderIF * SR);
gint sample_recorder_if_update_record(gpointer data);
/*
   void sample_recorder_if_name_changed(GtkWidget *widget, GdkEventKey *event, gpointer data);
 */
void sample_recorder_if_add_sample(GtkWidget * widget, gpointer data);

GType sample_recorder_if_get_type(void)
{
	static GType b_type = 0;

	if (!b_type) {
		static const GTypeInfo b_info = {
			sizeof(SampleRecorderIFClass),
			NULL,	/* base_init */
			NULL,	/* base_finalise */
			(GClassInitFunc) sample_recorder_if_class_init,
			NULL,	/* class_finalize */
			NULL,	/* class_data */
			sizeof(SampleRecorderIF),
			0,	/* n_preallocs */
			(GInstanceInitFunc) sample_recorder_if_init,
		};

		b_type = g_type_register_static(MODULEWINDOW_TYPE,
						"SampleRecorderIF",
						&b_info, 0);
	}
	return b_type;
}

static void sample_recorder_if_class_init(SampleRecorderIFClass * klass)
{

}

static void sample_recorder_if_init(SampleRecorderIF * sample_recorder_if)
{
}

GtkWidget *sample_recorder_if_new(sample_recorder * mod)
{
	SampleRecorderIF *sample_recorder_if;
	GtkWidget *vbox, *vbox2, *hbox, *hbox3;
	GtkWidget *button;
	GtkWidget *notebook, *label;
	GSList *rbgroup;

	sample_recorder_if = gtk_type_new(sample_recorder_if_get_type());

	modulewindow_set_module (MODULEWINDOW(sample_recorder_if), (module *)mod);

	sample_recorder_if->recordfunc_tag = 0;

	vbox2 = MODULEWINDOW(sample_recorder_if)->mainbox;
	hbox = MODULEWINDOW(sample_recorder_if)->headbox;

	button = outputlabel_new((module *)mod, 0);
	gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 2);
	gtk_widget_show(button);

	button = inputoption_new((char *) "In:", (module *)mod, 0);
	gtk_box_pack_start(GTK_BOX(vbox2), button, FALSE, TRUE, 2);
	gtk_widget_show(button);

	button = inputoption_new((char *) "Trigger:", (module *)mod, 1);
	gtk_box_pack_start(GTK_BOX(vbox2), button, FALSE, TRUE, 2);
	gtk_widget_show(button);


	/* NOTEBOOK */

	notebook = gtk_notebook_new();
	gtk_box_pack_start(GTK_BOX(vbox2), notebook, TRUE, TRUE, 0);
	gtk_widget_show(notebook);

	hbox = gtk_hbox_new(FALSE, 0);
	label = gtk_label_new("Record");
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), hbox, label);
	gtk_widget_show(hbox);

	vbox = gtk_vbox_new(FALSE, 5);
	gtk_box_pack_start(GTK_BOX(hbox), vbox, TRUE, TRUE, 0);
	gtk_widget_show(vbox);

	button = gtk_check_button_new_with_label("Record on next trigger");
	gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 2);
	gtk_toggle_button_set_state(GTK_TOGGLE_BUTTON(button), mod->record_next);
	g_signal_connect(G_OBJECT(button), "clicked",
			 G_CALLBACK(sample_recorder_if_record_next),
			 sample_recorder_if);
	gtk_widget_show(button);
	sample_recorder_if->record_next_check = button;

	hbox3 = gtk_hbox_new(FALSE, 5);
	gtk_box_pack_start(GTK_BOX(vbox), hbox3, TRUE, FALSE, 0);
	gtk_widget_show(hbox3);

	button = gtk_toggle_button_new_with_label("Record");
	gtk_box_pack_start(GTK_BOX(hbox3), button, FALSE, FALSE, 0);
	gtk_toggle_button_set_state(GTK_TOGGLE_BUTTON(button), mod->recording);
	g_signal_connect(G_OBJECT(button), "clicked",
			 G_CALLBACK(sample_recorder_if_record),
			 sample_recorder_if);
	gtk_widget_show(button);
	sample_recorder_if->record_toggle = button;

	button = gtk_button_new_with_label("Stop");
	gtk_box_pack_start(GTK_BOX(hbox3), button, FALSE, FALSE, 0);
	g_signal_connect(G_OBJECT(button), "clicked",
			 G_CALLBACK(sample_recorder_if_stop),
			 sample_recorder_if);
	gtk_widget_show(button);

	hbox3 = gtk_hbox_new(FALSE, 5);
	gtk_box_pack_start(GTK_BOX(vbox), hbox3, TRUE, FALSE, 0);
	gtk_widget_show(hbox3);

	button = gtk_progress_bar_new();
	gtk_box_pack_start(GTK_BOX(hbox3), button, FALSE, TRUE, 0);
	gtk_widget_show(button);
	sample_recorder_if->record_pbar = button;

	vbox = gtk_vbox_new(FALSE, 0);
	label = gtk_label_new("Playback");
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), vbox, label);
	gtk_widget_show(vbox);

	button =
	    gtk_radio_button_new_with_label(NULL,
					    "Always pass input through");
	gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 2);
	gtk_toggle_button_set_state(GTK_TOGGLE_BUTTON(button),
				    (mod->passthrough_mode ==
				     AUBE_SAMPLE_RECORDER_PASSTHROUGH_ALWAYS));
	g_signal_connect(G_OBJECT(button), "clicked",
			 G_CALLBACK(sample_recorder_if_passthrough_always),
			 mod);
	gtk_widget_show(button);

	rbgroup = gtk_radio_button_group(GTK_RADIO_BUTTON(button));

	button =
	    gtk_radio_button_new_with_label(rbgroup,
					    "Pass through when recording,\nelse playback");
	gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 2);
	gtk_toggle_button_set_state(GTK_TOGGLE_BUTTON(button),
				    (mod->passthrough_mode ==
				     AUBE_SAMPLE_RECORDER_PASSTHROUGH_REC));
	g_signal_connect(G_OBJECT(button), "clicked",
			 G_CALLBACK(sample_recorder_if_passthrough_rec),
			 mod);
	gtk_widget_show(button);
	rbgroup = gtk_radio_button_group(GTK_RADIO_BUTTON(button));

	button =
	    gtk_radio_button_new_with_label(rbgroup, "Always playback");
	gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 2);
	gtk_toggle_button_set_state(GTK_TOGGLE_BUTTON(button),
				    (mod->passthrough_mode ==
				     AUBE_SAMPLE_RECORDER_PASSTHROUGH_NEVER));
	g_signal_connect(G_OBJECT(button), "clicked",
			 G_CALLBACK(sample_recorder_if_passthrough_never),
			 mod);
	gtk_widget_show(button);

	button = gtk_button_new_with_label("Restart");
	gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 0);
	g_signal_connect(G_OBJECT(button), "clicked",
			 G_CALLBACK(sample_recorder_if_restart),
			 mod);
	gtk_widget_show(button);

	vbox = gtk_vbox_new(FALSE, 0);

	label = gtk_label_new("Buffer");
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), vbox, label);
	gtk_widget_show(vbox);

#if 0
	hbox3 = gtk_hbox_new(FALSE, 5);
	gtk_box_pack_start(GTK_BOX(vbox), hbox3, TRUE, TRUE, 0);
	gtk_widget_show(hbox3);

	label = gtk_label_new("Length:");
	gtk_box_pack_start(GTK_BOX(hbox3), label, TRUE, TRUE, 0);
	gtk_widget_show(label);

	button = gtk_entry_new();
	gtk_box_pack_start(GTK_BOX(hbox3), button, TRUE, FALSE, 0);
	gtk_widget_show(button);
#endif

	hbox3 = gtk_hbox_new(FALSE, 5);
	gtk_box_pack_start(GTK_BOX(vbox), hbox3, TRUE, FALSE, 0);
	gtk_widget_show(hbox3);

	label = gtk_label_new("Name:");
	gtk_box_pack_start(GTK_BOX(hbox3), label, TRUE, TRUE, 0);
	gtk_widget_show(label);

	button = gtk_entry_new();
	gtk_box_pack_start(GTK_BOX(hbox3), button, TRUE, TRUE, 0);
/*
   g_signal_connect(G_OBJECT(button), "key_press_event",
   G_CALLBACK(sample_recorder_if_name_changed),
   button);
 */
	gtk_widget_show(button);
	sample_recorder_if->samplename_entry = button;

	button = gtk_button_new_with_label("Add to sample list");
	gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 0);
	g_signal_connect(G_OBJECT(button), "clicked",
			 G_CALLBACK(sample_recorder_if_add_sample),
			 sample_recorder_if);
	gtk_widget_show(button);

	return GTK_WIDGET(sample_recorder_if);
}

void sample_recorder_if_record(GtkWidget * widget, gpointer data)
{
	SampleRecorderIF *SR = (SampleRecorderIF *) data;
	sample_recorder *sr = (sample_recorder *)MODULEWINDOW(SR)->module;

	sr->recording = 1 - sr->recording;
	sample_recorder_if_check_record(SR);
}

void sample_recorder_if_stop(GtkWidget * widget, gpointer data)
{
	SampleRecorderIF *SR = (SampleRecorderIF *) data;
	sample_recorder *sr = (sample_recorder *)MODULEWINDOW(SR)->module;

	sample_recorder_stop_recording(sr);
	sample_recorder_if_update_record(SR);
}

void sample_recorder_if_record_next(GtkWidget * widget, gpointer data)
{
	SampleRecorderIF *SR = (SampleRecorderIF *) data;
	sample_recorder *sr = (sample_recorder *)MODULEWINDOW(SR)->module;

	sr->record_next = 1 - sr->record_next;
	sample_recorder_if_check_record(SR);
}

void
sample_recorder_if_passthrough_never(GtkWidget * widget, gpointer data)
{
	((sample_recorder *) data)->passthrough_mode =
	    AUBE_SAMPLE_RECORDER_PASSTHROUGH_NEVER;
}

void sample_recorder_if_passthrough_rec(GtkWidget * widget, gpointer data)
{
	((sample_recorder *) data)->passthrough_mode =
	    AUBE_SAMPLE_RECORDER_PASSTHROUGH_REC;
}

void
sample_recorder_if_passthrough_always(GtkWidget * widget, gpointer data)
{
	((sample_recorder *) data)->passthrough_mode =
	    AUBE_SAMPLE_RECORDER_PASSTHROUGH_ALWAYS;
}

void sample_recorder_if_restart(GtkWidget * widget, gpointer data)
{
	((sample_recorder *) data)->play_offset = 0;
}

void sample_recorder_if_check_record(SampleRecorderIF * SR)
{
	sample_recorder *sr = (sample_recorder *)MODULEWINDOW(SR)->module;

	if (!sr->recording && !sr->record_next) {
		gtk_idle_remove(SR->recordfunc_tag);
		SR->recordfunc_tag = 0;
	} else if (!SR->recordfunc_tag) {
		SR->recordfunc_tag =
		    gtk_idle_add((GtkFunction)
				 sample_recorder_if_update_record, SR);
	}
}

gint sample_recorder_if_update_record(gpointer data)
{
	SampleRecorderIF *SR = (SampleRecorderIF *) data;
	sample_recorder *sr = (sample_recorder *)MODULEWINDOW(SR)->module;
	gfloat percent;


	g_signal_handlers_block_matched(SR->record_next_check,
					G_SIGNAL_MATCH_DATA, 0, 0, NULL,
					NULL, SR);
	gtk_toggle_button_set_state(GTK_TOGGLE_BUTTON(SR->record_next_check),
				    sr->record_next);
	g_signal_handlers_unblock_matched(SR->record_next_check,
					  G_SIGNAL_MATCH_DATA, 0, 0, NULL,
					  NULL, SR);


	g_signal_handlers_block_matched(SR->record_toggle,
					G_SIGNAL_MATCH_DATA, 0, 0, NULL,
					NULL, SR);
	gtk_toggle_button_set_state(GTK_TOGGLE_BUTTON(SR->record_toggle),
				    sr->recording);
	g_signal_handlers_unblock_matched(SR->record_toggle,
					  G_SIGNAL_MATCH_DATA, 0, 0, NULL,
					  NULL, SR);

	if (!sr->recording && !sr->record_next) {
		gtk_idle_remove(SR->recordfunc_tag);
		SR->recordfunc_tag = 0;
		return 0;
	}
	if (sr->recording) {
		percent =
		    (gfloat) (sr->rec_offset) /
		    (gfloat) (AUBE_SR_BUFFER_TICKS * DEFAULT_TICK);
		gtk_progress_bar_update(GTK_PROGRESS_BAR(SR->record_pbar),
					percent);
	}

	return 1;
}

/*
   void sample_recorder_if_name_changed(GtkWidget *widget, GdkEventKey *event, gpointer data)
   {
   if(event->keyval == GDK_Return) {
   printf("%s\n", gtk_entry_get_text(GTK_ENTRY(data)));
   }
   }
 */

void sample_recorder_if_add_sample(GtkWidget * widget, gpointer data)
{
	SampleRecorderIF *SR = (SampleRecorderIF *) data;
	sample_recorder *sr = (sample_recorder *)MODULEWINDOW(SR)->module;
        const gchar * samplename;

        samplename = gtk_entry_get_text(GTK_ENTRY (SR->samplename_entry));
	sample_recorder_add_sample(sr, samplename);
}
