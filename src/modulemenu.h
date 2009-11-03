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

#ifndef __UNITMENU_H__
#define __UNITMENU_H__

#include <gdk/gdk.h>
#include <gtk/gtkvbox.h>
#include "aube.h"

G_BEGIN_DECLS

#define UNITMENU(obj)          GTK_CHECK_CAST (obj, modulemenu_get_type (), Unitmenu)
#define UNITMENU_CLASS(klass)  GTK_CHECK_CLASS_CAST (klass, modulemenu_get_type (), UnitmenuClass)
#define IS_UNITMENU(obj)       GTK_CHECK_TYPE (obj, modulemenu_get_type ())

typedef struct _Unitmenu Unitmenu;
typedef struct _UnitmenuClass UnitmenuClass;

typedef struct {
	module *this_module;
	module *selected_module;
} modulepack;

struct _Unitmenu {
	GtkMenu menu;

	int (*include_criterion) (module * u1, module * u2);
	void (*select_action) (GtkWidget * widget, gpointer data);
	modulepack *up;
};

struct _UnitmenuClass {
	GtkMenuClass parent_class;

	void (*modulemenu) (Unitmenu * a);
};

GType modulemenu_get_type(void);
GtkWidget *modulemenu_new(module * this_module,
			  int *include_criterion(),
			  void *select_action());

G_END_DECLS

#endif /* __UNITMENU_H__ */
