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
#include <stdio.h>
#include <strings.h>
#include <gtk/gtk.h>
#include "inputoption.h"

extern GtkWidget *master_daddy;
extern aube_data *aube_daddy;

extern bit16 zero_buffer[];


static void inputoption_class_init(InputoptionClass * klass);
static void inputoption_init(Inputoption * b);
GtkWidget *inputoption_new();
void inputoption_dnd_drop(GtkWidget * widget, GdkEvent * event);
void inputoption_dismiss(GtkWidget * widget, gpointer data);
void reread_inputs_cb(GtkWidget * widget, gpointer data);
void select_input_cb(GtkWidget * widget, gpointer data);

GType inputoption_get_type(void)
{
	static GType b_type = 0;

	if (!b_type) {
		static const GTypeInfo b_info = {
			sizeof(InputoptionClass),
			NULL,	/* base_init */
			NULL,	/* base_finalize */
			(GClassInitFunc) inputoption_class_init,
			NULL,	/* class_finalize */
			NULL,	/* class_data */
			sizeof(Inputoption),
			0,	/* n_preallocs */
			(GInstanceInitFunc) inputoption_init,
		};

		b_type = g_type_register_static(GTK_TYPE_FRAME,
						"Inputoption", &b_info, 0);
	}
	return b_type;
}

/* FIXME: need these?
enum {
  LAST_SIGNAL
};

static guint inputoption_signals[LAST_SIGNAL+1] =
{0};
*/
static void inputoption_class_init(InputoptionClass * klass)
{
/* FIXME: need these?	

  GtkObjectClass *object_class;

  object_class = (GtkObjectClass *) klass;

  gtk_object_class_add_signals(object_class, inputoption_signals, LAST_SIGNAL);
*/
	klass->inputoption = NULL;
}

static void inputoption_init(Inputoption * a)
{
}

GtkWidget *inputoption_new(char *label_str, module * module, int input_i)
{
	Inputoption *inputoption;
	GtkWidget *widget;
	GtkWidget *hbox;

#if 0
	char *accepted_drop_types[] = { "text/plain" };
#endif

	inputoption = gtk_type_new(inputoption_get_type());
	gtk_frame_set_shadow_type(GTK_FRAME(inputoption), GTK_SHADOW_IN);
	gtk_container_border_width(GTK_CONTAINER(inputoption), 0);

	inputoption->module = module;
	inputoption->input_i = input_i;
	inputoption->ip = NULL;

	gtk_container_border_width(GTK_CONTAINER(inputoption), 1);

	hbox = gtk_hbox_new(FALSE, 1);
	gtk_container_add(GTK_CONTAINER(inputoption), hbox);
	gtk_widget_show(hbox);

	widget = gtk_label_new(label_str);
	gtk_box_pack_start(GTK_BOX(hbox), widget, FALSE, FALSE, 1);
	gtk_widget_show(widget);

	inputoption->optionmenu = gtk_option_menu_new();
	gtk_box_pack_start(GTK_BOX(hbox), inputoption->optionmenu,
			   TRUE, TRUE, 0);
	gtk_widget_show(inputoption->optionmenu);

#if 0
	widget = gtk_button_new_with_label("Reread");
	gtk_box_pack_start(GTK_BOX(inputoption), widget, FALSE, FALSE, 0);
	g_signal_connect(G_OBJECT(widget), "clicked",
			 G_CALLBACK(reread_inputs_cb), inputoption);
	gtk_widget_show(widget);
#endif

	inputoption->handler_id = g_signal_connect(G_OBJECT(master_daddy),
						   "modules_changed",
						   G_CALLBACK
						   (reread_inputs_cb),
						   inputoption);

#if 0
	gtk_widget_realize(hbox);
	g_signal_connect(G_OBJECT(hbox), "drop_data_available_event",
			 G_CALLBACK(inputoption_dnd_drop), hbox);
	gtk_widget_dnd_drop_set(hbox, TRUE, accepted_drop_types, 1, FALSE);
#endif

	g_signal_connect(G_OBJECT(inputoption), "destroy",
			 G_CALLBACK(inputoption_dismiss), inputoption);

	return GTK_WIDGET(inputoption);
}

void inputoption_dnd_drop(GtkWidget * widget, GdkEvent * event)
{
/*
   fprintf(stderr, "Got: %s", (char *)event->dropdataavailable.data);
 */
}

void inputoption_dismiss(GtkWidget * widget, gpointer data)
{
	gtk_signal_disconnect(G_OBJECT(master_daddy),
			      INPUTOPTION(data)->handler_id);
	gtk_widget_destroy(GTK_WIDGET(data));
}

void reread_inputs_cb(GtkWidget * widget, gpointer data)
{
	int i, j, k = 0;
	int last_choice = 0;
	int ch_type;
	GtkWidget *menuitem;
	char buf[64];

	module *module;
	input *input;
	channel *channel;

	g_return_if_fail(IS_INPUTOPTION(data));

	module = INPUTOPTION(data)->module;
	g_assert(module != NULL);

	input = module->inputs[INPUTOPTION(data)->input_i];
	g_assert(input != NULL);

	ch_type = input->ch_type;

	INPUTOPTION(data)->inputsmenu = gtk_menu_new();

	if (INPUTOPTION(data)->ip != NULL) {
		free(INPUTOPTION(data)->ip);
	}
	if (ch_type == CH_TYPE_AUDIO_SAMPLE) {
		INPUTOPTION(data)->ip =
		    malloc(sizeof(inputpack) *
			   (aube_daddy->nr_samples + 1));
		for (i = -1; i < (int) (aube_daddy->nr_samples); i++) {
			if (i == -1) {
				snprintf(buf, sizeof(buf), "None");
				INPUTOPTION(data)->ip[k].ch = NULL;
			} else {
				snprintf(buf, sizeof(buf), "%s",
					 aube_daddy->samples[i]->u_label);
				INPUTOPTION(data)->ip[k].ch =
				    aube_daddy->samples[i];
			}

			if (input->channel == INPUTOPTION(data)->ip[k].ch) {
				last_choice = k;
			}
			menuitem = gtk_menu_item_new_with_label(buf);
			INPUTOPTION(data)->ip[k].data = data;
			gtk_menu_append(GTK_MENU
					(INPUTOPTION(data)->inputsmenu),
					menuitem);
			g_signal_connect(G_OBJECT(menuitem), "activate",
					 G_CALLBACK(select_input_cb),
					 &(INPUTOPTION(data)->ip[k]));
			gtk_widget_show(menuitem);
			k++;
		}
	} else {
		INPUTOPTION(data)->ip =
		    malloc(sizeof(inputpack) * (aube_daddy->nr_channels));

		for (i = 0; i < (int) (aube_daddy->nr_modules); i++) {
			g_assert(aube_daddy->modules[i] != NULL);

			for (j = 0;
			     j <
			     (int) (aube_daddy->modules[i]->nr_outputs);
			     j++) {
				channel =
				    aube_daddy->modules[i]->outputs[j];

				if ((channel != NULL)
				    && (channel->parms.ch_type == ch_type)
				    && (INPUTOPTION(data)->module !=
					channel->module)) {

					INPUTOPTION(data)->ip[k].ch =
					    channel;

					if (i == 0)
						snprintf(buf, sizeof(buf),
							 "None");
					else if (aube_daddy->modules[i]->
						 nr_outputs == 1)
						snprintf(buf, sizeof(buf),
							 "%s",
							 aube_daddy->
							 modules[i]->
							 u_label);
					else
						snprintf(buf, sizeof(buf),
							 "%s:%s",
							 aube_daddy->
							 modules[i]->
							 u_label,
							 INPUTOPTION
							 (data)->ip[k].ch->
							 u_label);

					if (input->channel == channel) {
						last_choice = k;
					}
					menuitem =
					    gtk_menu_item_new_with_label
					    (buf);
					INPUTOPTION(data)->ip[k].data =
					    data;
					gtk_menu_append(GTK_MENU
							(INPUTOPTION
							 (data)->
							 inputsmenu),
							menuitem);
					g_signal_connect(G_OBJECT
							 (menuitem),
							 "activate",
							 G_CALLBACK
							 (select_input_cb),
							 &(INPUTOPTION
							   (data)->ip[k]));
					gtk_widget_show(menuitem);
					k++;
				}
			}
		}
	}

	gtk_option_menu_remove_menu(GTK_OPTION_MENU
				    (INPUTOPTION(data)->optionmenu));
	gtk_option_menu_set_menu(GTK_OPTION_MENU
				 (INPUTOPTION(data)->optionmenu),
				 INPUTOPTION(data)->inputsmenu);
	gtk_option_menu_set_history(GTK_OPTION_MENU
				    (INPUTOPTION(data)->optionmenu),
				    last_choice);
}

void select_input_cb(GtkWidget * widget, gpointer data)
{
	inputpack *ip;

	ip = (inputpack *) data;
	((module *) INPUTOPTION(ip->data)->module)->
	    inputs[INPUTOPTION(ip->data)->input_i]->channel = ip->ch;

#ifdef DEBUG
	fprintf(stderr, "Set %x->inputs[%d] to %x (%s)\n",
		INPUTOPTION(ip->data)->module,
		INPUTOPTION(ip->data)->input_i, ip->ch, ip->ch->u_label);
#endif
}
