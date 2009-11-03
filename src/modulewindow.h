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

#ifndef __MODULEWINDOW_H__
#define __MODULEWINDOW_H__

#include <gdk/gdk.h>
#include <gtk/gtkwindow.h>
#include "aube.h"

G_BEGIN_DECLS

#define MODULEWINDOW_TYPE          (modulewindow_get_type ())
#define MODULEWINDOW(obj)          GTK_CHECK_CAST (obj, modulewindow_get_type (), ModuleWindow)
#define MODULEWINDOW_CLASS(klass)  GTK_CHECK_CLASS_CAST (klass, modulewindow_get_type (), ModuleWindowClass)
#define IS_MODULEWINDOW(obj)       GTK_CHECK_TYPE (obj, modulewindow_get_type ())


typedef struct _ModuleWindow ModuleWindow;
typedef struct _ModuleWindowClass ModuleWindowClass;

struct _ModuleWindow {
	GtkWindow window;

	module * module;
};

struct _ModuleWindowClass {
	GtkWindowClass parent_class;

	void (*modulewindow) (ModuleWindow * a);
};

GType modulewindow_get_type(void);
GtkWidget *modulewindow_new(module * module);
void modulewindow_set_module (ModuleWindow * mw, module * module);

G_END_DECLS

#endif	/* __MODULEWINDOW_H__ */
