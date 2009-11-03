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

#ifndef __MASTER_H__
#define __MASTER_H__

#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include "aube.h"
#include "opsmenu.h"

#ifdef __cplusplus
extern "C" {
#endif				/*
				   __cplusplus 
				 */


#define MASTER(obj)          GTK_CHECK_CAST (obj, master_get_type (), Master)
#define MASTER_CLASS(klass)  GTK_CHECK_CLASS_CAST (klass, master_get_type (), MasterClass)
#define IS_MASTER(obj)       GTK_CHECK_TYPE (obj, master_get_type ())


	typedef struct _Master Master;
	typedef struct _MasterClass MasterClass;

	struct _Master {
		GtkWindow window;

		aube_data *data;
	};

	struct _MasterClass {
		GtkWindowClass parent_class;

		void (*master) (Master * a);
	};

	GType master_get_type(void);
	GtkWidget *master_new(void);
	void master_ack_channels_modified(void);
	void module_config_cb(GtkWidget * widget, gpointer data);
	void module_clone_cb(GtkWidget * widget, gpointer data);
	void module_replace_cb(GtkWidget * widget, gpointer data);
	void change_replace_menu_cb(GtkWidget * widget, gpointer data);

#ifdef __cplusplus
}
#endif				/*
				   __cplusplus 
				 */
#endif				/*
				   __MASTER_H__ 
				 */
