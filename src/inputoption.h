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

#ifndef __INPUTOPTION_H__
#define __INPUTOPTION_H__

#include <gdk/gdk.h>
#include <gtk/gtkframe.h>
#include "aube.h"

G_BEGIN_DECLS

#define INPUTOPTION(obj)          GTK_CHECK_CAST (obj, inputoption_get_type (), Inputoption)
#define INPUTOPTION_CLASS(klass)  GTK_CHECK_CLASS_CAST (klass, inputoption_get_type (), InputoptionClass)
#define IS_INPUTOPTION(obj)       GTK_CHECK_TYPE (obj, inputoption_get_type ())


typedef struct _Inputoption Inputoption;
typedef struct _InputoptionClass InputoptionClass;

typedef struct {
	channel *ch;
	gpointer data;
} inputpack;

struct _Inputoption {
	GtkFrame frame;

	GtkWidget *optionmenu;
	GtkWidget *inputsmenu;
	module *module;
	int input_i;
	inputpack *ip;
	guint handler_id;
};

struct _InputoptionClass {
	GtkWindowClass parent_class;

	void (*inputoption) (Inputoption * a);
};

GType inputoption_get_type(void);
GtkWidget *inputoption_new(char *label_str, module * module,
			   int input_i);
void reread_inputs_cb(GtkWidget * widget, gpointer data);

G_END_DECLS

#endif /* __INPUTOPTION_H__ */
