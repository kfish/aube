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

#ifndef __NONE_CONFIG_H__
#define __NONE_CONFIG_H__

#include <gdk/gdk.h>
#include <gtk/gtk.h>

#include "aube.h"

G_BEGIN_DECLS

#define NONE_CONFIG(obj)          GTK_CHECK_CAST (obj, none_config_get_type (), NoneConfig)
#define NONE_CONFIG_CLASS(klass)  GTK_CHECK_CLASS_CAST (klass, none_config_get_type (), NoneConfigClass)
#define IS_NONE_CONFIG(obj)       GTK_CHECK_TYPE (obj, none_config_get_type ())

typedef struct _NoneConfig NoneConfig;
typedef struct _NoneConfigClass NoneConfigClass;

struct _NoneConfig {
	GtkWindow window;
};

struct _NoneConfigClass {
	GtkWindowClass parent_class;

	void (*none_config) (NoneConfig * a);
};

void none_config_cb(module * u);

GType none_config_get_type(void);
GtkWidget *none_config_new(module * u);

G_END_DECLS

#endif /* __NONE_CONFIG_H__ */
