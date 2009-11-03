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

#ifndef __OPSMENU_H__
#define __OPSMENU_H__

#include <gdk/gdk.h>
#include <gtk/gtkvbox.h>
#include "aube.h"

#ifdef __cplusplus
extern "C" {
#endif				/*
				   __cplusplus 
				 */


#define OPSMENU(obj)          GTK_CHECK_CAST (obj, opsmenu_get_type (), Opsmenu)
#define OPSMENU_CLASS(klass)  GTK_CHECK_CLASS_CAST (klass, opsmenu_get_type (), OpsmenuClass)
#define IS_OPSMENU(obj)       GTK_CHECK_TYPE (obj, opsmenu_get_type ())


	typedef struct _Opsmenu Opsmenu;
	typedef struct _OpsmenuClass OpsmenuClass;

	struct _Opsmenu {
		GtkMenuBar menubar;

		module *module;
	};

	struct _OpsmenuClass {
		GtkMenuBarClass parent_class;

		void (*opsmenu) (Opsmenu * a);
	};

	GType opsmenu_get_type(void);
	GtkWidget *opsmenu_new(module * module, GtkWidget * parent_if,
			       void *if_hide_cb, void *if_close_cb);


#ifdef __cplusplus
}
#endif				/*
				   __cplusplus 
				 */
#endif				/*
				   __OPSMENU_H__ 
				 */
