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

#ifndef __PANNER_IF_H__
#define __PANNER_IF_H__

#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include "modulewindow.h"
#include "panner.h"

G_BEGIN_DECLS

#define PANNER_IF(obj)          GTK_CHECK_CAST (obj, panner_if_get_type (), PannerIF)
#define PANNER_IF_CLASS(klass)  GTK_CHECK_CLASS_CAST (klass, panner_if_get_type (), PannerIFClass)
#define IS_PANNER_IF(obj)       GTK_CHECK_TYPE (obj, panner_if_get_type ())

typedef struct _PannerIF PannerIF;
typedef struct _PannerIFClass PannerIFClass;

struct _PannerIF {
	ModuleWindow window;
};

struct _PannerIFClass {
	ModuleWindowClass parent_class;

	void (*panner_if) (PannerIF * p);
};

GType panner_if_get_type(void);
GtkWidget *panner_if_new(panner * pn);
void panner_if_close_cb(GtkWidget * widget, gpointer data);

G_END_DECLS

#endif				/* __PANNER_IF_H__ */
