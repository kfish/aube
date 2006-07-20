#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <gtk/gtk.h>

#include "aube.h"
#include "master.h"
#include "sample.h"

extern GtkWidget *master_daddy;
extern gint master_signals[];

extern channel *selected_sample;

static char last_sample_dir[256];

void
sample_load_ok_cb(GtkWidget * widget, gpointer data)
{
  const gchar *dir;

  dir = gtk_file_selection_get_filename(GTK_FILE_SELECTION(data));
  strncpy(last_sample_dir, dir, MIN(256, strlen(dir)));
  sample_load(dir);

  /*
     remove filename from dir 
   */
  *(rindex(last_sample_dir, '/')) = '\0';

  master_ack_channels_modified();
/*
   gtk_widget_destroy(GTK_WIDGET(data));
 */
}

void
sample_load_cancel_cb(GtkWidget * widget, gpointer data)
{
  gtk_widget_destroy(GTK_WIDGET(data));
}

void
sample_load_help_cb(GtkWidget * widget, gpointer data)
{
}


void
sample_load_cb(GtkWidget * wiget, gpointer data)
{
  GtkWidget *filesel;

  chdir(last_sample_dir);

  filesel = gtk_file_selection_new("Load Sample");
  gtk_signal_connect(GTK_OBJECT(GTK_FILE_SELECTION(filesel)->ok_button),
		     "clicked", GTK_SIGNAL_FUNC(sample_load_ok_cb), filesel);
  gtk_signal_connect(GTK_OBJECT(GTK_FILE_SELECTION(filesel)->cancel_button),
		"clicked", GTK_SIGNAL_FUNC(sample_load_cancel_cb), filesel);
#if 0
  gtk_signal_connect(GTK_OBJECT(GTK_FILE_SELECTION(filesel)->help_button),
		  "clicked", GTK_SIGNAL_FUNC(sample_load_help_cb), filesel);
#endif

  gtk_widget_show(filesel);
}

void
sample_save_ok_cb(GtkWidget * widget, gpointer data)
{
  const gchar *dir;

  dir = gtk_file_selection_get_filename(GTK_FILE_SELECTION(data));
  strncpy(last_sample_dir, dir, MIN(256, strlen(dir)));
  sample_save(selected_sample, dir);

  /*
     remove filename from dir 
   */
  *(rindex(last_sample_dir, '/')) = '\0';

  master_ack_channels_modified();
/*
   gtk_widget_destroy(GTK_WIDGET(data));
 */
}

void
sample_save_cancel_cb(GtkWidget * widget, gpointer data)
{
  gtk_widget_destroy(GTK_WIDGET(data));
}

void
sample_save_help_cb(GtkWidget * widget, gpointer data)
{
}

void
sample_save_cb(GtkWidget * wiget, gpointer data)
{
  GtkWidget *filesel;

  chdir(last_sample_dir);

  filesel = gtk_file_selection_new("Save Sample");
  gtk_signal_connect(GTK_OBJECT(GTK_FILE_SELECTION(filesel)->ok_button),
		     "clicked", GTK_SIGNAL_FUNC(sample_save_ok_cb), filesel);
  gtk_signal_connect(GTK_OBJECT(GTK_FILE_SELECTION(filesel)->cancel_button),
		"clicked", GTK_SIGNAL_FUNC(sample_save_cancel_cb), filesel);
#if 0
  gtk_signal_connect(GTK_OBJECT(GTK_FILE_SELECTION(filesel)->help_button),
		  "clicked", GTK_SIGNAL_FUNC(sample_save_help_cb), filesel);
#endif

  gtk_widget_show(filesel);
}
