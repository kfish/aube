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

#ifndef __DRUMMACHINE_IF_H__
#define __DRUMMACHINE_IF_H__

#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include "modulewindow.h"
#include "drummachine.h"

G_BEGIN_DECLS

#define DRUMMACHINE_IF(obj)          GTK_CHECK_CAST (obj, drummachine_if_get_type (), DrumMachineIF)
#define DRUMMACHINE_IF_CLASS(klass)  GTK_CHECK_CLASS_CAST (klass, drummachine_if_get_type (), DrumMachineIFClass)
#define IS_DRUMMACHINE_IF(obj)       GTK_CHECK_TYPE (obj, drummachine_if_get_type ())

typedef struct _DrumMachineIF DrumMachineIF;
typedef struct _DrumMachineIFClass DrumMachineIFClass;

typedef struct {
	/*    int index; */
	GtkWidget *button;
} DrumButton;

struct _DrumMachineIF {
	ModuleWindow window;

	GtkWidget *clear_buttons[MAX_CHANNELS];
	DrumButton buttons[MAX_CHANNELS][DM_LENGTH];
};

struct _DrumMachineIFClass {
	ModuleWindowClass parent_class;

	void (*drummachine_if) (DrumMachineIF * a);
};

GType drummachine_if_get_type(void);
GtkWidget *drummachine_if_new(drummachine * rsq);
void drummachine_if_close_cb(GtkWidget * widget, gpointer data);

G_END_DECLS

#endif /* __DRUMMACHINE_IF_H__ */
