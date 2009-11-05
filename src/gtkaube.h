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

#ifndef __GTKAUBE_H__
#define __GTKAUBE_H__

#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include "modulewindow.h"
#include "opsmenu.h"
#include "inputoption.h"
#include "outputlabel.h"
#include "levelmeter.h"
#include "slider.h"

G_BEGIN_DECLS

#define GTKAUBE_BOILERPLATE(type,type_as_function) \
GType type_as_function ## _get_type(void)				\
{									\
	static GType _type = 0;						\
									\
	if (G_UNLIKELY(_type == 0)) {					\
		GTypeInfo *type_info;					\
		type_info = g_new0 (GTypeInfo, 1);			\
		type_info->class_size = sizeof(type ## Class);		\
		type_info->instance_size = sizeof(type);		\
		_type = g_type_register_static(MODULEWINDOW_TYPE,	\
						#type,			\
						type_info, 0);		\
		g_free (type_info);					\
	}								\
	return _type;							\
}

G_END_DECLS

#endif /* __GTKAUBE_H__ */
