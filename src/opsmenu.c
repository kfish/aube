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

#include <strings.h>
#include <gtk/gtk.h>
#include "opsmenu.h"
#include "master.h"

extern GtkWidget *master_daddy;
extern aube_data *aube_daddy;

extern bit16 zero_buffer[];

static void opsmenu_class_init(OpsmenuClass * klass);
static void opsmenu_init(Opsmenu * b);
GtkWidget *opsmenu_new();
void reread_inputs_cb(GtkWidget * widget, gpointer data);
void select_input_cb(GtkWidget * widget, gpointer data);

GType opsmenu_get_type(void)
{
	static GType b_type = 0;

	if (!b_type) {
		static const GTypeInfo b_info = {
			sizeof(OpsmenuClass),
			NULL,	/* base_init */
			NULL,	/* base_finalise */
			(GClassInitFunc) opsmenu_class_init,
			NULL,	/* class_finalize */
			NULL,	/* class_data */
			sizeof(Opsmenu),
			0,	/* n_preallocs */
			(GInstanceInitFunc) opsmenu_init,
		};

		b_type = g_type_register_static(GTK_TYPE_MENU_BAR,
						"Opsmenu", &b_info, 0);
	}
	return b_type;
}

static void opsmenu_class_init(OpsmenuClass * klass)
{

}

static void opsmenu_init(Opsmenu * a)
{
}

GtkWidget *opsmenu_new(module * module, GtkWidget * parent_if,
		       void *if_hide_cb, void *if_close_cb)
{
	Opsmenu *opsmenu;
	GtkWidget *menu, *menuitem;
	GtkWidget *arrow, *label, *arrow_hbox;

	opsmenu = gtk_type_new(opsmenu_get_type());

	opsmenu->module = module;

	/*
	   OPS MENU 
	 */

	menu = gtk_menu_new();

#if 0
	menuitem = gtk_menu_item_new_with_label("Configure...");
	gtk_menu_append(GTK_MENU(menu), menuitem);
	g_signal_connect(G_OBJECT(menuitem), "activate",
			 G_CALLBACK(module_config_cb), module);
	gtk_widget_show(menuitem);
#endif

	menuitem = gtk_menu_item_new_with_label("Hide");
	gtk_menu_append(GTK_MENU(menu), menuitem);
	g_signal_connect(G_OBJECT(menuitem), "activate",
			 G_CALLBACK(if_hide_cb), parent_if);
	gtk_widget_show(menuitem);

	menuitem = gtk_menu_item_new_with_label("Clone");
	gtk_menu_append(GTK_MENU(menu), menuitem);
	g_signal_connect(G_OBJECT(menuitem), "activate",
			 G_CALLBACK(module_clone_cb), module);
	gtk_widget_show(menuitem);

#if 0
	if (module->class) {
		module->class->replace_menuitem =
		    gtk_menu_item_new_with_label("Replace with");

		menu2 =
		    modulemenu_new(module, (void *) aube_module_cmp_type,
				   (void *) module_replace_cb);
		gtk_menu_item_set_submenu(GTK_MENU_ITEM(menuitem), menu2);

		gtk_menu_append(GTK_MENU(menu), menuitem);
		g_signal_connect(G_OBJECT(master_daddy), "modules_changed",
				 G_CALLBACK(change_replace_menu_cb),
				 module);
		gtk_widget_show(menuitem);
	} else {
		fprintf(stderr, "module->class==NULL\n");
	}
#endif

	menuitem = gtk_menu_item_new();
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(menuitem), menu);
	gtk_menu_bar_append(GTK_MENU_BAR(opsmenu), menuitem);

	arrow_hbox = gtk_hbox_new(FALSE, 1);
	gtk_container_add(GTK_CONTAINER(menuitem), arrow_hbox);
	gtk_widget_show(arrow_hbox);

	arrow = gtk_arrow_new(GTK_ARROW_DOWN, GTK_SHADOW_OUT);
	gtk_box_pack_start(GTK_BOX(arrow_hbox), arrow, FALSE, FALSE, 0);
	gtk_misc_set_alignment(GTK_MISC(arrow), 0.5, 0.5);
	gtk_widget_show(arrow);

	label = gtk_label_new(module->u_label);
	gtk_box_pack_start(GTK_BOX(arrow_hbox), label, FALSE, FALSE, 4);
	gtk_misc_set_alignment(GTK_MISC(label), 0.5, 0.5);
	gtk_widget_show(label);

	gtk_widget_show(menuitem);

	return GTK_WIDGET(opsmenu);
}

