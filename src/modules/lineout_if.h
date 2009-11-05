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

#ifndef __LINEOUT_IF_H__
#define __LINEOUT_IF_H__

#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include "modulewindow.h"
#include "line.h"

G_BEGIN_DECLS

#define LINEOUT_IF(obj)          GTK_CHECK_CAST (obj, lineout_if_get_type (), LineOutIF)
#define LINEOUT_IF_CLASS(klass)  GTK_CHECK_CLASS_CAST (klass, lineout_if_get_type (), LineOutIFClass)
#define IS_LINEOUT_IF(obj)       GTK_CHECK_TYPE (obj, lineout_if_get_type ())


typedef struct _LineOutIF LineOutIF;
typedef struct _LineOutIFClass LineOutIFClass;

struct _LineOutIF {
	ModuleWindow window;

	GtkWidget *inputs_om;
	GtkWidget *inputsmenu;
};

struct _LineOutIFClass {
	ModuleWindowClass parent_class;

	void (*lineout_if) (LineOutIF * a);
};

GType lineout_if_get_type(void);
GtkWidget *lineout_if_new(oss_out * oss_p);
void lineout_if_close_cb(GtkWidget * widget, gpointer data);

G_END_DECLS

#endif /* __LINEOUT_IF_H__ */
