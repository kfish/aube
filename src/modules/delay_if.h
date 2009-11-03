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

#ifndef __DELAY_IF_H__
#define __DELAY_IF_H__

#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include "modulewindow.h"
#include "delay.h"

G_BEGIN_DECLS

#define DELAY_IF(obj)          GTK_CHECK_CAST (obj, delay_if_get_type (), DelayIF)
#define DELAY_IF_CLASS(klass)  GTK_CHECK_CLASS_CAST (klass, delay_if_get_type (), DelayIFClass)
#define IS_DELAY_IF(obj)       GTK_CHECK_TYPE (obj, delay_if_get_type ())

typedef struct _DelayIF DelayIF;
typedef struct _DelayIFClass DelayIFClass;

struct _DelayIF {
	ModuleWindow window;

	GtkWidget *replace_menuitem;
	delay *data;
};

struct _DelayIFClass {
	ModuleWindowClass parent_class;

	void (*delay_if) (DelayIF * a);
};

GType delay_if_get_type(void);
GtkWidget *delay_if_new(delay * dl);
void delay_if_close_cb(GtkWidget * widget, gpointer data);

G_END_DECLS

#endif /* __DELAY_IF_H__ */
