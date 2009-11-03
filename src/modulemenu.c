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

#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <gtk/gtk.h>
#include "modulemenu.h"

extern GtkWidget *master_daddy;
extern aube_data *aube_daddy;

extern bit16 zero_buffer[];

static void modulemenu_class_init(UnitmenuClass * class);
static void modulemenu_init(Unitmenu * b);
GtkWidget *modulemenu_new(module * this_module, int *include_criterion(),
			  void *select_action());
void modulemenu_dismiss(GtkWidget * widget, gpointer data);
void reread_modules_cb(GtkWidget * widget, gpointer data);

GType modulemenu_get_type(void)
{
	static GType b_type = 0;

	if (!b_type) {
		static const GTypeInfo b_info = {
			sizeof(UnitmenuClass),
			NULL,	/* base_init */
			NULL,	/* base_finalise */
			(GClassInitFunc) modulemenu_class_init,
			NULL,	/* class_finalize */
			NULL,	/* class_data */
			sizeof(Unitmenu),
			0,	/* n_preallocs */
			(GInstanceInitFunc) modulemenu_init,
		};

		b_type = g_type_register_static(GTK_TYPE_MENU,
						"Unitmenu", &b_info, 0);
	}
	return b_type;
}

static void modulemenu_class_init(UnitmenuClass * class)
{
	/* chickens? */
}

static void modulemenu_init(Unitmenu * a)
{
}

GtkWidget *modulemenu_new(module * this_module, int *include_criterion(),
			  void *select_action())
{
	GtkWidget *modulemenu;
	GtkWidget *menuitem;
	module *u;
	int i, k = 0;
	char buf[64];

	modulemenu = gtk_type_new(modulemenu_get_type());

	((Unitmenu *) modulemenu)->include_criterion =
	    (void *) include_criterion;
	((Unitmenu *) modulemenu)->select_action = (void *) select_action;

	UNITMENU(modulemenu)->up =
	    malloc(sizeof(modulepack) * (aube_daddy->nr_modules));

	for (i = 1; i < (int) (aube_daddy->nr_modules); i++) {
		u = aube_daddy->modules[i];
		if (this_module != u
		    && UNITMENU(modulemenu)->include_criterion(this_module,
							       u)) {
			UNITMENU(modulemenu)->up[k].this_module =
			    this_module;
			UNITMENU(modulemenu)->up[k].selected_module = u;

			snprintf(buf, sizeof(buf), "%s", u->u_label);

			menuitem = gtk_menu_item_new_with_label(buf);
			gtk_menu_append(GTK_MENU(modulemenu), menuitem);
			g_signal_connect(G_OBJECT(menuitem), "activate",
					 G_CALLBACK(UNITMENU(modulemenu)->
						    select_action),
					 &UNITMENU(modulemenu)->up[k]);
			gtk_widget_show(menuitem);
			k++;
		}
	}
	if (k == 0) {
		snprintf(buf, sizeof(buf), "None");

		menuitem = gtk_menu_item_new_with_label(buf);
		gtk_widget_set_state(GTK_WIDGET(menuitem),
				     GTK_STATE_INSENSITIVE);
		gtk_menu_append(GTK_MENU(modulemenu), menuitem);
		gtk_widget_show(menuitem);
	}
	return GTK_WIDGET(modulemenu);
}

void modulemenu_dismiss(GtkWidget * widget, gpointer data)
{
	free(UNITMENU(data)->up);
	gtk_widget_destroy(GTK_WIDGET(data));
}
