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

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <libgen.h>
#include <gtk/gtk.h>

#include "aube.h"
#include "master.h"
#include "sample.h"
#include "preferences.h"

extern GtkWidget *master_daddy;
extern gint master_signals[];

extern channel *selected_sample;

#define RECENT_LOAD_DIR_KEY "Sample_Load_Dir_Recent"
#define RECENT_SAVE_DIR_KEY "Sample_Save_Dir_Recent"

void sample_load_ok_cb(GtkWidget * widget, gpointer data)
{
	const gchar *dir;

	dir = gtk_file_selection_get_filename(GTK_FILE_SELECTION(data));
	sample_load(dir);
        prefs_set_string (RECENT_LOAD_DIR_KEY, dirname((char *)dir));

	master_ack_channels_modified();
/*
   gtk_widget_destroy(GTK_WIDGET(data));
 */
}

void sample_load_cancel_cb(GtkWidget * widget, gpointer data)
{
	gtk_widget_destroy(GTK_WIDGET(data));
}

void sample_load_help_cb(GtkWidget * widget, gpointer data)
{
}


void sample_load_cb(GtkWidget * widget, gpointer data)
{
	GtkWidget *filesel;
        gchar * recent_dir;
        int status;

        recent_dir = prefs_get_string (RECENT_LOAD_DIR_KEY);
        if (recent_dir) {
	        status = chdir(recent_dir);
                /* Ignore chdir errors ... stay where we are in that case */
        }

	filesel = gtk_file_selection_new("Load Sample");

	g_signal_connect(G_OBJECT(GTK_FILE_SELECTION(filesel)->ok_button),
			 "clicked", G_CALLBACK(sample_load_ok_cb),
			 filesel);
	g_signal_connect(G_OBJECT
			 (GTK_FILE_SELECTION(filesel)->cancel_button),
			 "clicked", G_CALLBACK(sample_load_cancel_cb),
			 filesel);
#if 0
	g_signal_connect(G_OBJECT
			 (GTK_FILE_SELECTION(filesel)->help_button),
			 "clicked", G_CALLBACK(sample_load_help_cb),
			 filesel);
#endif

	gtk_widget_show(filesel);
}

void sample_save_ok_cb(GtkWidget * widget, gpointer data)
{
	const gchar *dir;

	dir = gtk_file_selection_get_filename(GTK_FILE_SELECTION(data));
	sample_save(selected_sample, dir);
        prefs_set_string (RECENT_SAVE_DIR_KEY, dirname((char *)dir));

	master_ack_channels_modified();
/*
   gtk_widget_destroy(GTK_WIDGET(data));
 */
}

void sample_save_cancel_cb(GtkWidget * widget, gpointer data)
{
	gtk_widget_destroy(GTK_WIDGET(data));
}

void sample_save_help_cb(GtkWidget * widget, gpointer data)
{
}

void sample_save_cb(GtkWidget * widget, gpointer data)
{
	GtkWidget *filesel;
        gchar * recent_dir;
        int status;

        recent_dir = prefs_get_string (RECENT_SAVE_DIR_KEY);
        if (recent_dir) {
	        status = chdir(recent_dir);
                /* Ignore chdir errors ... stay where we are in that case */
        }

	filesel = gtk_file_selection_new("Save Sample");
	g_signal_connect(G_OBJECT(GTK_FILE_SELECTION(filesel)->ok_button),
			 "clicked", G_CALLBACK(sample_save_ok_cb),
			 filesel);
	g_signal_connect(G_OBJECT
			 (GTK_FILE_SELECTION(filesel)->cancel_button),
			 "clicked", G_CALLBACK(sample_save_cancel_cb),
			 filesel);
#if 0
	g_signal_connect(G_OBJECT
			 (GTK_FILE_SELECTION(filesel)->help_button),
			 "clicked", G_CALLBACK(sample_save_help_cb),
			 filesel);
#endif

	gtk_widget_show(filesel);
}
