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

#ifndef __LINEIN_IF_H__
#define __LINEIN_IF_H__

#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include "modulewindow.h"
#include "line.h"

G_BEGIN_DECLS

#define LINEIN_IF(obj)          GTK_CHECK_CAST (obj, linein_if_get_type (), LineInIF)
#define LINEIN_IF_CLASS(klass)  GTK_CHECK_CLASS_CAST (klass, linein_if_get_type (), LineInIFClass)
#define IS_LINEIN_IF(obj)       GTK_CHECK_TYPE (obj, linein_if_get_type ())

typedef struct _LineInIF LineInIF;
typedef struct _LineInIFClass LineInIFClass;

struct _LineInIF {
	ModuleWindow window;

	GtkWidget *inputs_om;
	GtkWidget *inputsmenu;
};

struct _LineInIFClass {
	ModuleWindowClass parent_class;

	void (*linein_if) (LineInIF * a);
};

GType linein_if_get_type(void);
GtkWidget *linein_if_new(oss_in * oss_p);
void linein_if_close_cb(GtkWidget * widget, gpointer data);

G_END_DECLS

#endif	/* __LINEIN_IF_H__ */
