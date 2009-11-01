/*
   klav.c - GTK+ "Klav" Widget
   * Copyright (C) 1998 Simon Kågedal
   *
   * This program is free software; you can redistribute it and/or modify
   * it under the terms of the GNU General Public License as published by
   * the Free Software Foundation; either version 2 of the License, or
   * (at your option) any later version.
   *
   * This program is distributed in the hope that it will be useful,
   * but WITHOUT ANY WARRANTY; without even the implied warranty of
   * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   * GNU General Public License in the file COPYING for more details.
   *
   * created 1998-04-18 Simon Kågedal
 */

#include <stdio.h>
#include <gtk/gtksignal.h>
#include "klav.h"

static GtkDrawingAreaClass *parent_class = NULL;

/*
   Signals 
 */
enum {
	KLAVKEY_PRESS,
	KLAVKEY_RELEASE,
	/*
	   KLAVKEY_CHANGED, 
	 */
	LAST_SIGNAL
};

static guint klav_signals[LAST_SIGNAL + 1] = { 0 };

typedef void (*KlavSignal1) (GtkObject * object, gint arg1, gpointer data);

/*
   Klav Methods 
 */
static void klav_class_init(KlavClass * klass);
static void klav_init(Klav * klav);

/*
   GtkObject Methods 
 */
static void klav_destroy(GtkObject * object);

/*
   GtkWidget Methods 
 */
static void klav_realize(GtkWidget * widget);
static gint klav_expose(GtkWidget * widget, GdkEventExpose * event);
static gint klav_button_press(GtkWidget * widget, GdkEventButton * event);
static gint klav_button_release(GtkWidget * widget,
				GdkEventButton * event);
static gint klav_motion_notify(GtkWidget * widget, GdkEventMotion * event);

GType klav_get_type(void)
{
	static GType b_type = 0;

	if (!b_type) {
		static const GTypeInfo b_info = {
			sizeof(KlavClass),
			NULL,	/* base_init */
			NULL,	/* base_finalise */
			(GClassInitFunc) klav_class_init,
			NULL,	/* class_finalize */
			NULL,	/* class_data */
			sizeof(Klav),
			0,	/* n_preallocs */
			(GInstanceInitFunc) klav_init,
		};

		b_type = g_type_register_static(GTK_TYPE_DRAWING_AREA,
						"Klav", &b_info, 0);
	}
	return b_type;
}

static void klav_class_init(KlavClass * klass)
{
	GtkObjectClass *object_class;
	GtkWidgetClass *widget_class;

	object_class = (GtkObjectClass *) klass;
	widget_class = (GtkWidgetClass *) klass;

	parent_class = gtk_type_class(gtk_drawing_area_get_type());

	klav_signals[KLAVKEY_PRESS] =
	    g_signal_new("klavkey_press",
			 G_TYPE_FROM_CLASS(klass),
			 G_SIGNAL_RUN_FIRST,
			 G_STRUCT_OFFSET(KlavClass, klavkey_press),
			 NULL,
			 NULL,
			 g_cclosure_marshal_VOID__INT, GTK_TYPE_NONE, 1,
			 GTK_TYPE_INT);

	/* FIXME
	   gtk_signal_new("klavkey_press", GTK_RUN_FIRST, object_class->type,
	   GTK_SIGNAL_OFFSET(KlavClass, klavkey_press),
	   klav_marshal_signal_1, GTK_TYPE_NONE, 1,
	   GTK_TYPE_INT);
	 */
	klav_signals[KLAVKEY_RELEASE] =
	    g_signal_new("klavkey_release",
			 G_TYPE_FROM_CLASS(klass),
			 G_SIGNAL_RUN_FIRST,
			 G_STRUCT_OFFSET(KlavClass, klavkey_release),
			 NULL,
			 NULL,
			 g_cclosure_marshal_VOID__INT, GTK_TYPE_NONE, 1,
			 GTK_TYPE_INT);

	/* FIXME
	   gtk_signal_new("klavkey_release", GTK_RUN_FIRST, object_class->type,
	   GTK_SIGNAL_OFFSET(KlavClass, klavkey_release),
	   klav_marshal_signal_1, GTK_TYPE_NONE, 1,
	   GTK_TYPE_INT);
	 */

	object_class->destroy = klav_destroy;

	widget_class->realize = klav_realize;
	widget_class->expose_event = klav_expose;

	widget_class->button_press_event = klav_button_press;
	widget_class->button_release_event = klav_button_release;
	widget_class->motion_notify_event = klav_motion_notify;
}

static void klav_init(Klav * klav)
{
	klav->type = KLAV_TYPE_SEQUENCER;
	klav->dir = KLAV_DIR_HORIZONTAL;
	klav->key_start = 36;
	klav->key_end = 96;
	klav->black_key_height = 0.6;
	klav->black_key_width = 0.54;

	klav->is_pressed = FALSE;
	klav->key_pressed = 0;

	klav->key_info = NULL;
	klav->key_info_size = 0;
}

static void klav_destroy(GtkObject * object)
{
	Klav *klav;

	g_return_if_fail(object != NULL);
	g_return_if_fail(IS_KLAV(object));

	klav = KLAV(object);

	/*
	   eventually free memory allocated for key info
	 */

	if (GTK_OBJECT_CLASS(parent_class)->destroy)
		(*GTK_OBJECT_CLASS(parent_class)->destroy) (object);
	/* FIXME: double free occurs if this is enabled. 
	   if (klav->key_info) {
	   g_free(klav->key_info);
	   } */
}

/*
   checks if the given key is a black one
 */

static gboolean is_black_key(gint key)
{
	switch (key % 12) {
	case 1:
	case 3:
	case 6:
	case 8:
	case 10:
		return (TRUE);

	default:
		return (FALSE);
	}
}

/*
   calculates how many white and black keys there are in a certain
   * interval on the keyboard 
 */

static void calc_keys(gint start, gint end, gint * whites, gint * blacks)
{
	gint i;

	*blacks = 0;
	*whites = 0;

	if (end >= start) {
		for (i = start; i <= end; i++) {
			if (is_black_key(i))
				(*blacks)++;
		}

		*whites = end - start + 1 - *blacks;
	}
}

/*
   swaps x and y values if klav->dir == KLAV_DIR_VERTICAL
 */

static void dir_swap(Klav * klav, gint * x1, gint * y1)
{
	if (klav->dir == KLAV_DIR_VERTICAL) {
		gint x;
		x = *x1;
		*x1 = *y1;
		*y1 = x;
	}
}

/*
   use this swapping routine on all points, like on a line
 */

static void dir_swap_point(Klav * klav, gint * x1, gint * y1)
{
	if (klav->dir == KLAV_DIR_VERTICAL) {
		gint x;
		x = *x1;
		*x1 = *y1;
		*y1 = GTK_WIDGET(klav)->allocation.height - x;
	}
}

/*
   the opposite of dir_swap_point
 */

static void dir_swap_point_reverse(Klav * klav, gint * x1, gint * y1)
{
	if (klav->dir == KLAV_DIR_VERTICAL) {
		gint y;
		y = *y1;
		*y1 = *x1;
		*x1 = GTK_WIDGET(klav)->allocation.height - y;
	}
}

/*
   use this swapping routine on all rectangles
 */

static void
dir_swap_rectangle(Klav * klav, gint * x1, gint * y1, gint * width,
		   gint * height)
{
	if (klav->dir == KLAV_DIR_VERTICAL) {
		gint x;
		x = *x1;
		*x1 = *y1;
		*y1 = GTK_WIDGET(klav)->allocation.height - x - *width;

		x = *width;
		*width = *height;
		*height = x;
	}
}

/*
   this drawing routine lets each key, whether white or black, take
   * up the same width
 */

static void
calc_key_sequencer(Klav * klav, gint key, KlavKeyInfo * key_info)
{
	GtkWidget *widget;
	gint width, height;

	widget = GTK_WIDGET(klav);

	width = widget->allocation.width;
	height = widget->allocation.height;

	dir_swap(klav, &width, &height);

	switch (key % 12) {
	case 1:
	case 3:
	case 6:
	case 8:
	case 10:

		/*
		   black key 
		 */

		key_info->is_black = TRUE;
		key_info->upper_right_x =
		    (key - klav->key_start + 1) * klav->keywidth;

		break;

	case 0:
	case 2:
	case 5:
	case 7:
	case 9:

		/*
		   short right side 
		 */

		key_info->is_black = FALSE;
		key_info->upper_right_x =
		    ((gfloat) (key - klav->key_start) +
		     1) * klav->keywidth;
		key_info->lower_right_x =
		    ((gfloat) (key - klav->key_start) +
		     1.5) * klav->keywidth;

		break;

	case 4:
	case 11:

		/*
		   long right side 
		 */

		key_info->is_black = FALSE;
		key_info->upper_right_x =
		    key_info->lower_right_x =
		    (key - klav->key_start + 1) * klav->keywidth;

		break;
	}
}

/*
   this draws a key like on a real keyboard, y'know, all white keys have
   * the same width and the black keys are a bit narrower.
 */

static void calc_key_normal(Klav * klav, gint key, KlavKeyInfo * key_info)
{
	GtkWidget *widget;
	gint blacks, whites, width, height;

	widget = GTK_WIDGET(klav);

	width = widget->allocation.width;
	height = widget->allocation.height;

	dir_swap(klav, &width, &height);

	calc_keys(klav->key_start, key - 1, &whites, &blacks);

	switch (key % 12) {
	case 1:
	case 3:
	case 6:
	case 8:
	case 10:

		/*
		   black key 
		 */

		key_info->is_black = TRUE;
		key_info->upper_right_x =
		    ((gfloat) whites +
		     (klav->black_key_width * 0.5)) * klav->keywidth;

		break;

	case 0:
	case 2:
	case 5:
	case 7:
	case 9:

		/*
		   the (whites):th white key from left or bottom, with short
		   * right side
		 */

		key_info->is_black = FALSE;
		key_info->upper_right_x =
		    (whites + 1) * klav->keywidth -
		    (klav->black_key_width * 0.5 * klav->keywidth);
		key_info->lower_right_x = (whites + 1) * klav->keywidth;

		break;

	case 4:
	case 11:

		/*
		   the (whites):th white key from left or bottom, with long
		   * right side
		 */

		key_info->is_black = FALSE;
		key_info->upper_right_x =
		    key_info->lower_right_x =
		    (whites + 1) * klav->keywidth;

		break;

	}
}

static void calc_key_info(Klav * klav)
{
	gint keys = klav->key_end - klav->key_start + 1;
	gint i;

	if (!klav->key_info) {
		klav->key_info = g_malloc(sizeof(KlavKeyInfo) * keys);
	} else if (klav->key_info_size != keys) {
		klav->key_info =
		    g_realloc(klav->key_info, sizeof(KlavKeyInfo) * keys);
	}
	for (i = 0; i < keys; i++) {
		if (klav->type == KLAV_TYPE_NORMAL) {
			calc_key_normal(klav, klav->key_start + i,
					&klav->key_info[i]);
		} else {
			calc_key_sequencer(klav, klav->key_start + i,
					   &klav->key_info[i]);
		}

		/*
		   fix so draw_key draws lower rectangle correctly 
		 */

		if (i > 0 && (klav->key_info[i].is_black)) {
			klav->key_info[i].lower_right_x =
			    klav->key_info[i - 1].lower_right_x;
		}
	}
}

/*
   main drawing function
 */

static gint klav_expose(GtkWidget * widget, GdkEventExpose * event)
{
	Klav *klav;
	int i;
	gint keys, whitekeys, blackkeys;
	gint width, height;
	KlavKeyInfo first = { 0, 0 }, *prev;

	g_return_val_if_fail(widget != NULL, FALSE);
	g_return_val_if_fail(IS_KLAV(widget), FALSE);
	g_return_val_if_fail(event != NULL, FALSE);

	klav = KLAV(widget);

	width = widget->allocation.width;
	height = widget->allocation.height;

	dir_swap(klav, &width, &height);

	keys = klav->key_end - klav->key_start + 1;

	calc_keys(klav->key_start, klav->key_end, &whitekeys, &blackkeys);

	switch (klav->type) {
	case KLAV_TYPE_SEQUENCER:
		klav->keywidth = (gfloat) width / (gfloat) (keys);
		break;

	case KLAV_TYPE_NORMAL:
		klav->keywidth = (gfloat) width / (gfloat) whitekeys;
		break;
	}

	calc_key_info(klav);

	/*
	   first.upper_right_x = first.lower_right_x = 0;
	 */
	prev = &first;

	for (i = 0; i < keys; i++) {
		gint x1, y1, x2, y2;

		if (klav->key_info[i].is_black) {
			x1 = prev->upper_right_x + 1;
			y1 = 0;
			x2 = klav->key_info[i].upper_right_x -
			    prev->upper_right_x - 1;
			y2 = height * klav->black_key_height;

			dir_swap_rectangle(klav, &x1, &y1, &x2, &y2);

			gdk_draw_rectangle(widget->window,
					   widget->style->black_gc,
					   TRUE, x1, y1, x2, y2);
		} else {
			x1 = klav->key_info[i].lower_right_x;
			y1 = 0;
			x2 = x1;
			y2 = height;

			dir_swap_point(klav, &x1, &y1);
			dir_swap_point(klav, &x2, &y2);

			gdk_draw_line(widget->window,
				      widget->style->black_gc,
				      x1, y1, x2, y2);
		}

		prev = &klav->key_info[i];
	}

	return FALSE;
}

/*
   draws a key pressed or unpressed
 */

static void draw_key(Klav * klav, gint key, gboolean pressed)
{
	KlavKeyInfo first = { 0, 0 }, *prev, *this;
	gint keynum;
	gint x1, y1, x2, y2, width, height;
	GtkWidget *widget;
	GdkGC *gc;

	widget = GTK_WIDGET(klav);

	width = widget->allocation.width;
	height = widget->allocation.height;

	dir_swap(klav, &width, &height);

	keynum = key - klav->key_start;

	this = &(klav->key_info[keynum]);
	prev = (keynum > 0) ? &(klav->key_info[keynum - 1]) : &first;

	if (pressed) {
		gc = widget->style->bg_gc[1];
	} else {
		gc = this->is_black ? widget->style->black_gc :
		    widget->style->bg_gc[0];
	}

	/*
	   draw upper part 
	 */

	x1 = prev->upper_right_x + 1;
	y1 = 0;
	x2 = this->upper_right_x - prev->upper_right_x - 1;
	y2 = height * klav->black_key_height;

	dir_swap_rectangle(klav, &x1, &y1, &x2, &y2);

	gdk_draw_rectangle(widget->window, gc, TRUE, x1, y1, x2, y2);

	if (!klav->key_info[keynum].is_black) {
		/*
		   draw lower part 
		 */

		x1 = prev->lower_right_x + 1;
		y1 = height * klav->black_key_height;
		x2 = this->lower_right_x - prev->lower_right_x - 1;
		y2 = height * (1.0 - klav->black_key_height);

		dir_swap_rectangle(klav, &x1, &y1, &x2, &y2);

		gdk_draw_rectangle(widget->window,
				   gc, TRUE, x1, y1, x2, y2);
	}
}

/*
   see which key is drawn at x, y
 */

static gint which_key(Klav * klav, gint x, gint y)
{
	gint i, keys, width, height;

	width = GTK_WIDGET(klav)->allocation.width;
	height = GTK_WIDGET(klav)->allocation.height;

	dir_swap_point_reverse(klav, &x, &y);
	/*
	   dir_swap (klav, &x, &y);
	 */

	dir_swap(klav, &width, &height);

	keys = klav->key_end - klav->key_start + 1;

	if (y > (height * klav->black_key_height)) {
		/*
		   check lower part 
		 */

		for (i = 0; i < keys; i++) {
			if (x < klav->key_info[i].lower_right_x) {
				return klav->key_start + i;
			}
		}
	} else {
		/*
		   check upper part 
		 */

		for (i = 0; i < keys; i++) {
			if (x < klav->key_info[i].upper_right_x) {
				return (klav->key_start + i);
			}
		}
	}

	/*
	   failed... 
	 */

	return -1;
}

static void press_key(Klav * klav, gint key)
{
	klav->is_pressed = TRUE;
	klav->key_pressed = key;

	g_signal_emit(G_OBJECT(klav), klav_signals[KLAVKEY_PRESS], 0, key);

	/*
	   printf("press: %i\n", key); 
	 */
}

static void release_key(Klav * klav)
{
	klav->is_pressed = FALSE;

	g_signal_emit(G_OBJECT(klav), klav_signals[KLAVKEY_RELEASE], 0,
		      klav->key_pressed);

	/*
	   printf ("release: %i\n", klav->key_pressed); 
	 */
}

/*
   events
 */

static gint klav_button_press(GtkWidget * widget, GdkEventButton * event)
{
	Klav *klav;
	gint key;

	g_return_val_if_fail(widget != NULL, FALSE);
	g_return_val_if_fail(IS_KLAV(widget), FALSE);
	g_return_val_if_fail(event != NULL, FALSE);

	klav = KLAV(widget);

	key = which_key(klav, (gint) event->x, (gint) event->y);

	press_key(klav, key);

	return FALSE;
}

static gint klav_button_release(GtkWidget * widget, GdkEventButton * event)
{
	gint key;
	Klav *klav;

	g_return_val_if_fail(widget != NULL, FALSE);
	g_return_val_if_fail(IS_KLAV(widget), FALSE);
	g_return_val_if_fail(event != NULL, FALSE);

	klav = KLAV(widget);

	key = which_key(klav, (gint) event->x, (gint) event->y);

	release_key(klav);

	return FALSE;
}

static gint klav_motion_notify(GtkWidget * widget, GdkEventMotion * event)
{
	Klav *klav;
	gint key;

	g_return_val_if_fail(widget != NULL, FALSE);
	g_return_val_if_fail(IS_KLAV(widget), FALSE);
	g_return_val_if_fail(event != NULL, FALSE);

	klav = KLAV(widget);

	if (klav->is_pressed) {
		key = which_key(klav, (gint) event->x, (gint) event->y);

		if (key != klav->key_pressed) {
			release_key(klav);
			press_key(klav, key);
		}
	}
	/*
	   printf("motion: %f %f\n", event->x, event->y);
	 */

	return FALSE;
}

GtkWidget *klav_new(void)
{
	Klav *klav;

	klav = gtk_type_new(klav_get_type());

	/*
	   old_mask = gtk_widget_get_events (GTK_WIDGET (klav)); 
	 */
	/*
	   gtk_widget_set_events (GTK_WIDGET (klav), old_mask |  
	 */

	return GTK_WIDGET(klav);
}

static void klav_realize(GtkWidget * widget)
{
	GdkWindowAttr attributes;
	Klav *klav;
	gint attributes_mask;

	g_return_if_fail(widget != NULL);
	g_return_if_fail(IS_KLAV(widget));

	klav = KLAV(widget);
	GTK_WIDGET_SET_FLAGS(widget, GTK_REALIZED);

	attributes.window_type = GDK_WINDOW_CHILD;
	attributes.x = widget->allocation.x;
	attributes.y = widget->allocation.y;
	attributes.width = widget->allocation.width;
	attributes.height = widget->allocation.height;
	attributes.wclass = GDK_INPUT_OUTPUT;
	attributes.visual = gtk_widget_get_visual(widget);
	attributes.colormap = gtk_widget_get_colormap(widget);
	attributes.event_mask = gtk_widget_get_events(widget) |
	    GDK_KEY_PRESS_MASK | GDK_KEY_RELEASE_MASK |
	    GDK_EXPOSURE_MASK | GDK_BUTTON_PRESS_MASK |
	    GDK_BUTTON_RELEASE_MASK | GDK_POINTER_MOTION_MASK;
	/*
	   GDK_POINTER_MOTION_HINT_MASK
	 */

	attributes_mask =
	    GDK_WA_X | GDK_WA_Y | GDK_WA_VISUAL | GDK_WA_COLORMAP;

	widget->window =
	    gdk_window_new(gtk_widget_get_parent_window(widget),
			   &attributes, attributes_mask);
	gdk_window_set_user_data(widget->window, klav);

	widget->style = gtk_style_attach(widget->style, widget->window);
	gtk_style_set_background(widget->style, widget->window,
				 GTK_STATE_NORMAL);
}

/*
   these routines should most often be mapped to the press and release
   * signals, but not all applications would want it that way
 */

void klav_press(Klav * klav, gint key)
{
	/*
	   TODO: should keep track of what keys are pressed so that the
	   * whole widget can be correctly redrawn
	 */

	draw_key(klav, key, TRUE);
}

void klav_release(Klav * klav, gint key)
{
	draw_key(klav, key, FALSE);
}

/*
   attribute setting 
 */

void klav_set_range(Klav * klav, gint start, gint end)
{
	klav->key_start = start;
	klav->key_end = end;
}

void klav_set_klav_type(Klav * klav, KlavType type)
{
	klav->type = type;
}

void klav_set_klav_dir(Klav * klav, KlavDir dir)
{
	klav->dir = dir;
}
