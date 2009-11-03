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

#ifndef __KLAVIER_IF_H__
#define __KLAVIER_IF_H__

#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include "modulewindow.h"
#include "klavier.h"

G_BEGIN_DECLS

#define KLAVIER_IF(obj)          GTK_CHECK_CAST (obj, klavier_if_get_type (), KlavierIF)
#define KLAVIER_IF_CLASS(klass)  GTK_CHECK_CLASS_CAST (klass, klavier_if_get_type (), KlavierIFClass)
#define IS_KLAVIER_IF(obj)       GTK_CHECK_TYPE (obj, klavier_if_get_type ())

typedef struct _KlavierIF KlavierIF;
typedef struct _KlavierIFClass KlavierIFClass;

struct _KlavierIF {
	ModuleWindow window;
};

struct _KlavierIFClass {
	ModuleWindowClass parent_class;

	void (*klavier_if) (KlavierIF * a);
};

GType klavier_if_get_type(void);
GtkWidget *klavier_if_new(klavier * k);
void klavier_if_close_cb(GtkWidget * widget, gpointer data);

G_END_DECLS

#endif /* __KLAVIER_IF_H__ */
