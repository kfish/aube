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

#ifndef __MINIMAUBE_IF_H__
#define __MINIMAUBE_IF_H__

#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include "modulewindow.h"
#include "minimaube.h"

G_BEGIN_DECLS

#define MINIMAUBE_IF(obj)          GTK_CHECK_CAST (obj, minimaube_if_get_type (), MiniMaubeIF)
#define MINIMAUBE_IF_CLASS(klass)  GTK_CHECK_CLASS_CAST (klass, minimaube_if_get_type (), MiniMaubeIFClass)
#define IS_MINIMAUBE_IF(obj)       GTK_CHECK_TYPE (obj, minimaube_if_get_type ())


typedef struct _MiniMaubeIF MiniMaubeIF;
typedef struct _MiniMaubeIFClass MiniMaubeIFClass;

struct _MiniMaubeIF {
	ModuleWindow window;

	GtkWidget *replace_menuitem;
	GtkWidget *hboxes[MAX_CHANNELS];
	minimaube *data;
};

struct _MiniMaubeIFClass {
	ModuleWindowClass parent_class;

	void (*minimaube_if) (MiniMaubeIF * a);
};

GType minimaube_if_get_type(void);
GtkWidget *minimaube_if_new(minimaube * dl);
void minimaube_if_close_cb(GtkWidget * widget, gpointer data);

G_END_DECLS

#endif /* __MINIMAUBE_IF_H__ */
