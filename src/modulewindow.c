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

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>
#include "modulewindow.h"
#include "opsmenu.h"

extern GtkWidget *master_daddy;
extern aube_data *aube_daddy;

static void modulewindow_class_init(ModuleWindowClass * klass);
static void modulewindow_init(ModuleWindow * b);
GtkWidget *modulewindow_new();

GType modulewindow_get_type(void)
{
	static GType mw_type = 0;

	if (!mw_type) {
		static const GTypeInfo mw_info = {
			sizeof(ModuleWindowClass),
			NULL,	/* base_init */
			NULL,	/* base_finalise */
			(GClassInitFunc) modulewindow_class_init,
			NULL,	/* class_finalize */
			NULL,	/* class_data */
			sizeof(ModuleWindow),
			0,	/* n_preallocs */
			(GInstanceInitFunc) modulewindow_init,
		};

		mw_type = g_type_register_static(GTK_TYPE_WINDOW,
						 "ModuleWindow", &mw_info,
						 0);
	}
	return mw_type;
}

static void modulewindow_class_init(ModuleWindowClass * klass)
{
}

static void modulewindow_destroy_cb (GtkWidget * widget, gpointer data)
{
        ModuleWindow * mw = (ModuleWindow *)data;

	aube_remove_module (mw->module);

        /* XXX: simply free the module data */
        free (mw->module);
}

static void modulewindow_hide_cb (GtkWidget * widget, gpointer data)
{
        ModuleWindow * mw = (ModuleWindow *)data;

	aube_module_remove_if(mw->module);
        gtk_widget_destroy (GTK_WIDGET(mw));
}

static void
modulewindow_close_cb(GtkAccelGroup * accel_group,
		      GObject * acceleratable,
		      guint keyval, GdkModifierType modifier)
{
	if (GTK_IS_WINDOW(acceleratable))
                gtk_widget_destroy (GTK_WIDGET(acceleratable));
		//g_signal_emit_by_name(G_OBJECT(acceleratable), "destroy");
}

static void modulewindow_onoff_cb(GtkWidget * widget, gpointer data)
{
	aube_module_toggle((module *) data);
}

static void modulewindow_toggle(GtkWidget * widget, gpointer data)
{
	ModuleWindow *mw = (ModuleWindow *) data;

	if (mw->module)
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON
					     (mw->onbutton),
					     !(mw->module->on));
}

static void modulewindow_init(ModuleWindow * mw)
{
	GClosure *gclosure;
	GtkAccelGroup *accel_group;

	g_signal_connect(G_OBJECT(mw), "destroy",
			 G_CALLBACK(modulewindow_destroy_cb),
			 mw);

        gtk_window_set_default_size (GTK_WINDOW(mw), MODULEWINDOW_DEFAULT_WIDTH, MODULEWINDOW_DEFAULT_HEIGHT);

	gtk_container_border_width(GTK_CONTAINER(mw), 1);

	accel_group = gtk_accel_group_new();
	gtk_window_add_accel_group(GTK_WINDOW(mw), accel_group);

	/*
	 * close the window when Ctrl and w are pressed.
	 *      accel key combo is static. perhaps there is a better
	 *      way to do this?
	 */
	gclosure =
	    g_cclosure_new((GCallback) modulewindow_close_cb, NULL, NULL);
	gtk_accel_group_connect(accel_group, GDK_w, GDK_CONTROL_MASK, 0,
				gclosure);

	/* Set up the boxes */
	mw->mainbox = gtk_vbox_new(FALSE, 5);
	gtk_container_add(GTK_CONTAINER(mw), mw->mainbox);
	gtk_widget_show(mw->mainbox);

	mw->headbox = gtk_hbox_new(FALSE, 5);
	gtk_box_pack_start(GTK_BOX(mw->mainbox), mw->headbox, FALSE, TRUE,
			   0);
	gtk_widget_show(mw->headbox);

        /* On button */
	mw->onbutton = gtk_toggle_button_new_with_label("On");
	gtk_box_pack_start(GTK_BOX(mw->headbox), mw->onbutton, FALSE,
			   FALSE, 1);
	gtk_widget_show(mw->onbutton);

	gclosure = g_cclosure_new((GCallback) modulewindow_toggle, mw, NULL);
	gtk_accel_group_connect(accel_group, GDK_Escape, 0, 0, gclosure);

}

#if 0
static void modulewindow_dismiss(GtkWidget * widget, gpointer data)
{
	gtk_widget_destroy(GTK_WIDGET(data));
}
#endif

void modulewindow_set_module(ModuleWindow * mw, module * module)
{
        GtkWidget * opsmenu;

	mw->module = module;

	gtk_window_set_title(GTK_WINDOW(mw), module->u_label);

	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(mw->onbutton),
				     module->on);
	g_signal_connect(G_OBJECT(mw->onbutton), "clicked",
			 G_CALLBACK(modulewindow_onoff_cb), module);

        /* Opsmenu */
	opsmenu = opsmenu_new(module, GTK_WIDGET(mw), modulewindow_hide_cb);
	gtk_box_pack_start(GTK_BOX(mw->headbox), opsmenu, FALSE, FALSE, 4);
	gtk_widget_show(opsmenu);

}

GtkWidget *modulewindow_new(module * module)
{
	ModuleWindow *mw;

	mw = g_object_new(MODULEWINDOW_TYPE, NULL);
	if (mw == NULL)
		return NULL;

	modulewindow_set_module(mw, module);

	return GTK_WIDGET(mw);
}
