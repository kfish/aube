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

#ifndef __PANNER4_IF_H__
#define __PANNER4_IF_H__

#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include "modulewindow.h"
#include "panner4.h"

G_BEGIN_DECLS

#define PANNER4_IF(obj)          GTK_CHECK_CAST (obj, panner4_if_get_type (), Panner4IF)
#define PANNER4_IF_CLASS(klass)  GTK_CHECK_CLASS_CAST (klass, panner4_if_get_type (), Panner4IFClass)
#define IS_PANNER4_IF(obj)       GTK_CHECK_TYPE (obj, panner4_if_get_type ())


typedef struct _Panner4IF Panner4IF;
typedef struct _Panner4IFClass Panner4IFClass;

struct _Panner4IF {
	ModuleWindow window;

	panner4 *data;
};

struct _Panner4IFClass {
	ModuleWindowClass parent_class;

	void (*panner4_if) (Panner4IF * a);
};

GType panner4_if_get_type(void);
GtkWidget *panner4_if_new(panner4 * pn);
void panner4_if_close_cb(GtkWidget * widget, gpointer data);

G_END_DECLS

#endif				/* __PANNER4_IF_H__ */
