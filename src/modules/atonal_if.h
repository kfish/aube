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

#ifndef __ATONAL_IF_H__
#define __ATONAL_IF_H__

#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include "modulewindow.h"
#include "atonal.h"

G_BEGIN_DECLS

#define ATONAL_IF(obj)          GTK_CHECK_CAST (obj, atonal_if_get_type (), AtonalIF)
#define ATONAL_IF_CLASS(klass)  GTK_CHECK_CLASS_CAST (klass, atonal_if_get_type (), AtonalIFClass)
#define IS_ATONAL_IF(obj)       GTK_CHECK_TYPE (obj, atonal_if_get_type ())

typedef struct _AtonalIF AtonalIF;
typedef struct _AtonalIFClass AtonalIFClass;

typedef struct {
	int index;
	GtkWidget *button;
} AtButton;

struct _AtonalIF {
	ModuleWindow window;

	AtButton buttons[AT_LENGTH];
};

struct _AtonalIFClass {
	ModuleWindowClass parent_class;

	void (*atonal_if) (AtonalIF * a);
};

GType atonal_if_get_type(void);
GtkWidget *atonal_if_new(atonal * rsq);
void atonal_if_close_cb(GtkWidget * widget, gpointer data);

G_END_DECLS

#endif /* __ATONAL_IF_H__ */
