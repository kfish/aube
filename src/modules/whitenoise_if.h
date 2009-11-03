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

#ifndef __WHITENOISE_IF_H__
#define __WHITENOISE_IF_H__

#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include "modulewindow.h"
#include "whitenoise.h"

G_BEGIN_DECLS

#define WHITENOISE_IF(obj)          GTK_CHECK_CAST (obj, whitenoise_if_get_type (), WhitenoiseIF)
#define WHITENOISE_IF_CLASS(klass)  GTK_CHECK_CLASS_CAST (klass, whitenoise_if_get_type (), WhitenoiseIFClass)
#define IS_WHITENOISE_IF(obj)       GTK_CHECK_TYPE (obj, whitenoise_if_get_type ())

typedef struct _WhitenoiseIF WhitenoiseIF;
typedef struct _WhitenoiseIFClass WhitenoiseIFClass;

struct _WhitenoiseIF {
	ModuleWindow window;
};

struct _WhitenoiseIFClass {
	ModuleWindowClass parent_class;

	void (*whitenoise_if) (WhitenoiseIF * a);
};

GType whitenoise_if_get_type(void);
GtkWidget *whitenoise_if_new(whitenoise * wn);
void whitenoise_if_close_cb(GtkWidget * widget, gpointer data);

G_END_DECLS

#endif /* __WHITENOISE_IF_H__ */
