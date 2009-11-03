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

#ifndef __FILTERRESLP_IF_H__
#define __FILTERRESLP_IF_H__

#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include "modulewindow.h"
#include "filter_reslp.h"

G_BEGIN_DECLS

#define FILTERRESLP_IF(obj)          GTK_CHECK_CAST (obj, filter_reslp_if_get_type (), FilterResLP_IF)
#define FILTERRESLP_IF_CLASS(klass)  GTK_CHECK_CLASS_CAST (klass, filter_reslp_if_get_type (), FilterResLP_IFClass)
#define IS_FILTERRESLP_IF(obj)       GTK_CHECK_TYPE (obj, filter_reslp_if_get_type ())


typedef struct _FilterResLP_IF FilterResLP_IF;
typedef struct _FilterResLP_IFClass FilterResLP_IFClass;

struct _FilterResLP_IF {
	ModuleWindow window;

	GtkWidget *resonance_env_curve;
	GtkWidget *cutoff_env_curve;

	gint env_tag;
	filter_reslp *data;
};

struct _FilterResLP_IFClass {
	ModuleWindowClass parent_class;

	void (*filter_reslp_if) (FilterResLP_IF * a);
};

GType filter_reslp_if_get_type(void);
GtkWidget *filter_reslp_if_new(filter_reslp * fr);
void filter_reslp_if_close_cb(GtkWidget * widget, gpointer data);

G_END_DECLS

#endif /* __FILTERRESLP_IF_H__ */
