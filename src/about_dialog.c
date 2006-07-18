#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <gtk/gtk.h>

#include <config.h>
#include "about_dialog.h"

static int about_dialog_load_logo(GtkWidget * window);
static void about_dialog_destroy(void);
static int about_dialog_logo_expose(GtkWidget * widget, GdkEventExpose * event);
static int about_dialog_button(GtkWidget * widget, GdkEventButton * event);

static GtkWidget *about_dialog = NULL;
static GtkWidget *logo_area = NULL;
static GdkPixmap *logo_pixmap = NULL;
static int logo_width = 0;
static int logo_height = 0;

void
about_dialog_create()
{
  GtkStyle *style;
  GtkWidget *vbox;
  GtkWidget *aboutframe;
  GtkWidget *label;
  GtkWidget *alignment;

  if (!about_dialog) {
    about_dialog = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_wmclass(GTK_WINDOW(about_dialog), "about_dialog", "Aube");
    gtk_window_set_title(GTK_WINDOW(about_dialog), "About AUBE");
    gtk_window_set_policy(GTK_WINDOW(about_dialog), FALSE, FALSE, FALSE);
    gtk_window_position(GTK_WINDOW(about_dialog), GTK_WIN_POS_CENTER);
    gtk_signal_connect(GTK_OBJECT(about_dialog), "destroy",
		       (GtkSignalFunc) about_dialog_destroy, NULL);
    gtk_signal_connect(GTK_OBJECT(about_dialog), "button_press_event",
		       (GtkSignalFunc) about_dialog_button, NULL);
    gtk_widget_set_events(about_dialog, GDK_BUTTON_PRESS_MASK);

    if (!about_dialog_load_logo(about_dialog)) {
      gtk_widget_destroy(about_dialog);
      about_dialog = NULL;
      return;
    }
    vbox = gtk_vbox_new(FALSE, 1);
    gtk_container_border_width(GTK_CONTAINER(vbox), 1);
    gtk_container_add(GTK_CONTAINER(about_dialog), vbox);
    gtk_widget_show(vbox);

    aboutframe = gtk_frame_new(NULL);
    gtk_frame_set_shadow_type(GTK_FRAME(aboutframe), GTK_SHADOW_IN);
    gtk_container_border_width(GTK_CONTAINER(aboutframe), 0);
    gtk_box_pack_start(GTK_BOX(vbox), aboutframe, TRUE, TRUE, 0);
    gtk_widget_show(aboutframe);

    logo_area = gtk_drawing_area_new();
    gtk_signal_connect(GTK_OBJECT(logo_area), "expose_event",
		       (GtkSignalFunc) about_dialog_logo_expose, NULL);
    gtk_drawing_area_size(GTK_DRAWING_AREA(logo_area), logo_width, logo_height);
    gtk_widget_set_events(logo_area, GDK_EXPOSURE_MASK);
    gtk_container_add(GTK_CONTAINER(aboutframe), logo_area);
    gtk_widget_show(logo_area);

    gtk_widget_realize(logo_area);
    gdk_window_set_background(logo_area->window, &logo_area->style->black);


    style = gtk_style_new();
/* FIXME: use pango or remove entirely?
    gdk_font_unref(style->font);
    style->font = gdk_font_load("-Adobe-Helvetica-Medium-R-Normal--*-140-*-*-*-*-*-*");
    gtk_widget_push_style(style);
*/
    label = gtk_label_new("This is AUBE Version " VERSION);
    gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, TRUE, 0);
    gtk_widget_show(label);

    label = gtk_label_new("Copyright (c) 1998 - 2002 Conrad Parker, conrad@metadecks.org");
    gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, TRUE, 0);
    gtk_widget_show(label);
/* FIXME 
    gtk_widget_pop_style();
*/
    alignment = gtk_alignment_new(0.5, 0.5, 0.0, 0.0);
    gtk_box_pack_start(GTK_BOX(vbox), alignment, FALSE, TRUE, 0);
    gtk_widget_show(alignment);

#ifndef HAVE_LIBSNDFILE
    label = gtk_label_new("This copy of AUBE was built without sample support\ndue to libsndfile being unavailable on the build host");
    gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, TRUE, 0);
    gtk_widget_show(label);

    alignment = gtk_alignment_new(0.5, 0.5, 0.0, 0.0);
    gtk_box_pack_start(GTK_BOX(vbox), alignment, FALSE, TRUE, 0);
    gtk_widget_show(alignment);
#endif

    label = gtk_label_new("Visit http://www.metadecks.org/software/aube/ for more info");
    gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, TRUE, 0);
    gtk_widget_show(label);

  }
  if (!GTK_WIDGET_MAPPED(about_dialog)) {
    gtk_widget_show(about_dialog);
  } else {
    gdk_window_raise(about_dialog->window);
  }
}


static int
about_dialog_load_logo(GtkWidget * window)
{
  GtkWidget *preview;
  GdkGC *gc;
  char buf[1024];
  unsigned char *pixelrow;
  FILE *fp;
  int count;
  int i;

  if (logo_pixmap)
    return TRUE;

  sprintf(buf, "%s/aube_logo.ppm", DATADIR);

  fp = fopen(buf, "rb");
  if (!fp)
    return 0;

  fgets(buf, 1024, fp);
  if (strcmp(buf, "P6\n") != 0) {
    fclose(fp);
    return 0;
  }
  fgets(buf, 1024, fp);
  fgets(buf, 1024, fp);
  sscanf(buf, "%d %d", &logo_width, &logo_height);

  fgets(buf, 1024, fp);
  if (strcmp(buf, "255\n") != 0) {
    fclose(fp);
    return 0;
  }
  preview = gtk_preview_new(GTK_PREVIEW_COLOR);
  gtk_preview_size(GTK_PREVIEW(preview), logo_width, logo_height);
  pixelrow = g_new(guchar, logo_width * 3);

  for (i = 0; i < logo_height; i++) {
    count = fread(pixelrow, sizeof(unsigned char), logo_width * 3, fp);
    if (count != (logo_width * 3)) {
      gtk_widget_destroy(preview);
      g_free(pixelrow);
      fclose(fp);
      return 0;
    }
    gtk_preview_draw_row(GTK_PREVIEW(preview), pixelrow, 0, i, logo_width);
  }

  gtk_widget_realize(window);
  logo_pixmap = gdk_pixmap_new(window->window, logo_width, logo_height,
			       gtk_preview_get_visual()->depth);
  gc = gdk_gc_new(logo_pixmap);
  gtk_preview_put(GTK_PREVIEW(preview),
		  logo_pixmap, gc,
		  0, 0, 0, 0, logo_width, logo_height);
  gdk_gc_destroy(gc);

  gtk_widget_unref(preview);
  g_free(pixelrow);

  fclose(fp);

  return TRUE;
}

static void
about_dialog_destroy()
{
  about_dialog = NULL;
}

static int
about_dialog_logo_expose(GtkWidget * widget,
			 GdkEventExpose * event)
{
  /*
     If we draw beyond the boundaries of the pixmap, then X
     will generate an expose area for those areas, starting
     an infinite cycle. We now set allow_grow = FALSE, so
     the drawing area can never be bigger than the preview.
     Otherwise, it would be necessary to intersect event->area
     with the pixmap boundary rectangle. 
   */

  gdk_draw_pixmap(widget->window, widget->style->black_gc, logo_pixmap,
		  event->area.x, event->area.y,
		  event->area.x, event->area.y,
		  event->area.width, event->area.height);

  return FALSE;
}

static int
about_dialog_button(GtkWidget * widget,
		    GdkEventButton * event)
{
  gtk_widget_hide(about_dialog);

  return FALSE;
}
