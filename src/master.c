#include <stdio.h>
#include <string.h>
#include <gdk_imlib.h>
#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>

#include <config.h>

#include "aube.h"
#include "master.h"

#include "about_dialog.h"
#include "modulemenu.h"
#include "slider.h"
#include "sample_if.h"
#if 0
#include "aubewave.h"
#endif
#include "none_config.h"

#if 0
#include "file_if.h"
#endif

#define __DISABLE_MINITRACKER
/* ... while it is a piece of shit */


#include "modules/linein_if.h"
#include "modules/lineout_if.h"
#include "modules/atonal_if.h"
#include "modules/delay_if.h"
#include "modules/drummachine_if.h"
#include "modules/filter_reslp_if.h"
#include "modules/general_mixer_if.h"
#include "modules/klavier_if.h"
#include "modules/minimaube_if.h"
#include "modules/panner_if.h"
#include "modules/panner4_if.h"
#ifndef __DISABLE_MINITRACKER
#include "modules/minitracker_if.h"
#endif
#include "modules/sample_recorder_if.h"
#include "modules/syre_if.h"
#include "modules/whitenoise_if.h"
#include "modules/xfader_if.h"

extern GtkWidget *master_daddy;
extern aube_data *aube_daddy;

extern int vol;
extern int pan;
extern int bpm;
extern int tick;

extern gint aube_do_tick(gpointer data);

GtkWidget *samplelist_daddy;

channel *selected_sample;

static GtkMenu * commands_menu;

#if 0
char *module_category_labels[LAST_UNIT_TYPE];
module_type_labels[CONTROLLER] = (char *) "Controllers";
module_type_labels[SEQUENCER] = (char *) "Sequencers";
module_type_labels[GENERATOR] = (char *) "Generators";
module_type_labels[FILTER] = (char *) "Filters";
module_type_labels[EFFECT] = (char *) "Effects";
module_type_labels[MIXER] = (char *) "Mixers";
module_type_labels[DRIVER] = (char *) "Outputs";
#endif

char *module_category_labels[] =
{
  "Controllers",
  "Sequencers",
  "Generators",
  "Filters",
  "Effects",
  "Mixers",
  "Outputs"
};

module_class *module_classes[] =
{
  &klavier_mif,
  &atonal_mif,
  &drummachine_mif,
#ifndef __DISABLE_MINITRACKER
  &minitracker_mif,
#endif
  &linein_mif,
  &minimaube_mif,
  &syre_mif,
  &whitenoise_mif,
  &filter_reslp_mif,
  &delay_mif,
  &panner_mif,
  &panner4_mif,
  &general_mixer_mif,
  &xfader_mif,
  &sample_recorder_mif,
  &lineout_mif,
};
int nr_module_classes = (sizeof(module_classes) / sizeof(module_class *));


static void master_class_init(MasterClass * class);
static void master_init(Master * b);
GtkWidget *master_new();
void master_dismiss(GtkWidget * widget, gpointer data);

void quit_cb(GtkWidget * widget, gpointer data);
void delay_toggle_cb(GtkWidget * widget, gpointer data);
gint delete_event(GtkWidget * widget, gpointer data);
void destroy(GtkWidget * widget, gpointer data);
void open_sample_cb(GtkWidget * widget, gpointer data);
void new_module_cb(GtkWidget * widget, gpointer data);
void change_unhide_menu_cb(GtkWidget * widget, gpointer data);
int include_unhidden(module * this_module, module * selected_module);
void select_unhidden(GtkWidget * widget, gpointer data);
void aube_samplelist_create(void);
void clear_workspace_cb(GtkWidget * widget, gpointer data);
void open_workspace_cb(GtkWidget * widget, gpointer data);
void save_workspace_cb(GtkWidget * widget, gpointer data);

GType
master_get_type(void)
{
  static GType b_type = 0;

  if (!b_type) {
    static const GTypeInfo b_info =
    {
      sizeof(MasterClass),
      NULL, /* base_init */
	  NULL, /* base_finalise */
      (GClassInitFunc) master_class_init,
	  NULL, /* class_finalize */
	  NULL, /* class_data */
      sizeof(Master),
	  0, /* n_preallocs */
	  (GInstanceInitFunc) master_init,
    };

    b_type = g_type_register_static(GTK_TYPE_WINDOW,
                                                      "Master",
	                                                   &b_info, 0);
  }
  return b_type;
}

enum {
  UNITS_CHANGED_SIGNAL,
  LAST_SIGNAL
};

/* static gint master_signals[LAST_SIGNAL] = { 0 }; */
guint master_signals[LAST_SIGNAL] = {0};

static void
master_class_init(MasterClass * klass)
{
  GtkObjectClass *object_class;

  object_class = (GtkObjectClass *) klass;

  master_signals[UNITS_CHANGED_SIGNAL] =
    g_signal_new("modules_changed",
	                     G_TYPE_FROM_CLASS(klass),
	                     G_SIGNAL_RUN_FIRST,
                         G_STRUCT_OFFSET(MasterClass, master),
                         NULL,
	                     NULL,
		                 g_cclosure_marshal_VOID__VOID, GTK_TYPE_NONE, 0);
}

static void
aube_refresh_samplelist(GtkWidget * widget, gpointer * data)
{
  int i;
  gchar *title[3];
  gchar buf1[16];
  gchar buf2[16];
  GtkCList * clist = GTK_CLIST (data);

  gtk_clist_freeze(clist);
  gtk_clist_clear(clist);
  title[0] = buf1;
  title[2] = buf2;
  for (i = 0; i < aube_daddy->nr_samples; i++) {
    snprintf(title[0], sizeof(title[0]), "%02d", i);
    title[1] = aube_daddy->samples[i]->u_label;
    snprintf(title[2], sizeof(title[2]), "%d",
	    aube_daddy->samples[i]->parms.d.audio_sample_data.s_length);
    gtk_clist_append(clist, (gchar **) & title);
  }
  gtk_clist_thaw(clist);
}

static gint
aube_samplelist_events(GtkWidget * widget, GdkEventButton * bevent, gpointer data)
{
  GtkWidget *clist;
  gint row, col;

  clist = GTK_WIDGET (data);

  if (bevent->button == 3) {
    gtk_clist_get_selection_info(GTK_CLIST(clist), bevent->x, bevent->y,
				 &row, &col);
    selected_sample = aube_daddy->samples[row];
    gtk_menu_popup(GTK_MENU(commands_menu), NULL, NULL, NULL,
		   aube_daddy->samples[row], 3, bevent->time);
  }
  return 1;
}

static void
aube_remove_sample_cb (GtkWidget * widget, gpointer data)
{
  aube_remove_sample(selected_sample);
}

static void
master_init(Master * master)
{
  GtkWidget *menubar, *menu, *menu2, *menuitem;
  /*  GtkWidget *module_type_menus[LAST_UNIT_TYPE];*/
  GtkWidget *frame;
  GtkWidget *hbox, *vbox, *vbox2;
  GtkWidget *slider;
  GtkWidget * button;
  int i;

  GtkAccelGroup * accel_group;

  GtkWidget * im;


  GtkWidget * scrolled;
  GtkWidget * clist;
  gchar *titles[] = {"", "Name", "Length"};

  master->data = aube_data_new();
  aube_daddy = master->data;

  g_signal_connect (G_OBJECT (master), "destroy",
		      G_CALLBACK (quit_cb), master);

  gtk_window_set_title(GTK_WINDOW(master), "AUBE/Metadecks Live");

  /* Create a GtkAccelGroup and add it to the window. */
  accel_group = gtk_accel_group_new();
  gtk_window_add_accel_group (GTK_WINDOW(master), accel_group);

  vbox2 = gtk_vbox_new(FALSE, 5);
  gtk_container_add(GTK_CONTAINER(master), vbox2);
  gtk_widget_show(vbox2);


  /*
     M E N U B A R 
   */

  menubar = gtk_menu_bar_new();
  gtk_box_pack_start(GTK_BOX(vbox2), menubar, FALSE, TRUE, 0);

  /*
     FILE MENU 
   */

  menu = gtk_menu_new();

  menuitem = gtk_menu_item_new_with_label("Load Sample...");
  gtk_menu_append(GTK_MENU(menu), menuitem);
  gtk_signal_connect(GTK_OBJECT(menuitem), "activate",
		     GTK_SIGNAL_FUNC(sample_load_cb), NULL);
  gtk_widget_show(menuitem);
  gtk_widget_add_accelerator (menuitem, "activate", accel_group,
                              GDK_l, GDK_CONTROL_MASK,
                              GTK_ACCEL_VISIBLE);

#if 0
  menuitem = gtk_menu_item_new_with_label("Samples...");
  gtk_menu_append(GTK_MENU(menu), menuitem);
#if HAVE_LIBSNDFILE
  gtk_signal_connect(GTK_OBJECT(menuitem), "activate",
		     GTK_SIGNAL_FUNC(aube_samplelist_create), NULL);
#else
  gtk_widget_set_state(GTK_WIDGET(menuitem), GTK_STATE_INSENSITIVE);
#endif
  gtk_widget_show(menuitem);
#endif

  menuitem = gtk_menu_item_new_with_label("Clear Workspace");
  gtk_menu_append(GTK_MENU(menu), menuitem);
  gtk_signal_connect(GTK_OBJECT(menuitem), "activate",
		     GTK_SIGNAL_FUNC(clear_workspace_cb), NULL);
  gtk_widget_show(menuitem);
  
  menuitem = gtk_menu_item_new();
  gtk_menu_append(GTK_MENU(menu), menuitem);
  gtk_widget_show(menuitem);

  menuitem = gtk_menu_item_new_with_label("About...");
  gtk_menu_append(GTK_MENU(menu), menuitem);
  g_signal_connect(G_OBJECT(menuitem), "activate",
		     G_CALLBACK(about_dialog_create), NULL);
  gtk_widget_show(menuitem);

  menuitem = gtk_menu_item_new();
  gtk_menu_append(GTK_MENU(menu), menuitem);
  gtk_widget_show(menuitem);

  menuitem = gtk_menu_item_new_with_label("Quit");
  gtk_menu_append(GTK_MENU(menu), menuitem);
  gtk_signal_connect(GTK_OBJECT(menuitem), "activate",
		     GTK_SIGNAL_FUNC(quit_cb), master);
  gtk_widget_show(menuitem);
  gtk_widget_add_accelerator (menuitem, "activate", accel_group,
                              GDK_q, GDK_CONTROL_MASK,
                              GTK_ACCEL_VISIBLE);

  menuitem = gtk_menu_item_new_with_label("File");
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(menuitem), menu);
  gtk_menu_bar_append(GTK_MENU_BAR(menubar), menuitem);
  gtk_widget_show(menuitem);


  /*
     MODULES MENU 
   */
  menu = gtk_menu_new();

#if 0
  menu2 = gtk_menu_new();
#endif
  menu2 = menu;

#if 0
  for (i = 0; i < LAST_UNIT_TYPE; i++) {
    menuitem = gtk_menu_item_new_with_label((char *) module_category_labels[i]);
    gtk_menu_append(GTK_MENU(menu2), menuitem);
    gtk_widget_show(menuitem);

    module_type_menus[i] = gtk_menu_new();
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(menuitem), module_type_menus[i]);
  }

  for (i = 0; i < (sizeof(module_classes) / sizeof(module_class *)); i++) {
    menuitem = gtk_menu_item_new_with_label(module_classes[i]->u_name);
    gtk_menu_append(GTK_MENU(module_type_menus[module_classes[i]->category]), menuitem);
    gtk_signal_connect(GTK_OBJECT(menuitem), "activate",
		       GTK_SIGNAL_FUNC(new_module_cb), module_classes[i]);
    gtk_widget_show(menuitem);
  }
#endif

#if 0
  menuitem = gtk_menu_item_new_with_label("New");
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(menuitem), menu2);
  gtk_menu_append(GTK_MENU(menu), menuitem);
  gtk_widget_show(menuitem);
#endif

#if 0
  menuitem = gtk_menu_item_new();
  widget = gtk_hseparator_new();
  gtk_container_add(GTK_CONTAINER(menuitem), widget);
  gtk_widget_show(widget);

  gtk_menu_append(GTK_MENU(menu), menuitem);
  gtk_widget_show(menuitem);
#endif

  menuitem = gtk_menu_item_new_with_label("Show Hidden");
  menu2 = modulemenu_new(NULL, (void *) include_unhidden, (void *) select_unhidden);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(menuitem), menu2);
  gtk_menu_append(GTK_MENU(menu), menuitem);
  gtk_signal_connect(GTK_OBJECT(master), "modules_changed",
		     GTK_SIGNAL_FUNC(change_unhide_menu_cb), menuitem);
  gtk_widget_show(menuitem);

  menuitem = gtk_menu_item_new_with_label("Modules");
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(menuitem), menu);
  gtk_menu_bar_append(GTK_MENU_BAR(menubar), menuitem);
  gtk_widget_show(menuitem);

  gtk_widget_show(menubar);

   im = gtk_image_new_from_file (g_strconcat (DATADIR, "/aube_mini.png", NULL));
    
   gtk_box_pack_start(GTK_BOX(vbox2), im, FALSE, TRUE, 0);
   gtk_widget_show (im);
  

  /*
     M A S T E R 
   */


  hbox = gtk_hbox_new(FALSE, 0);
  gtk_box_pack_start (GTK_BOX(vbox2), hbox, TRUE, TRUE, 0);
  gtk_widget_show(hbox);


  /* BPM SLIDER */

  frame = gtk_frame_new(NULL);
  gtk_frame_set_shadow_type (GTK_FRAME (frame), GTK_SHADOW_IN);
  gtk_box_pack_start(GTK_BOX(hbox), frame, TRUE, TRUE, 8);
  gtk_container_border_width(GTK_CONTAINER(frame), 8);
  gtk_widget_show(frame);

  slider = slider_int_new("BPM", &bpm, 30, 240, 1);
  gtk_container_add (GTK_CONTAINER (frame), slider);
  gtk_widget_show(slider);


  /* MODULE BUTTONS */

  vbox = gtk_vbox_new(FALSE, 5);
  gtk_box_pack_start(GTK_BOX(hbox), vbox, FALSE, TRUE, 2);  
  gtk_widget_show(vbox);

  for (i = 0; i < (sizeof(module_classes) / sizeof(module_class *)); i++) {
    button = gtk_button_new_with_label (module_classes[i]->u_name);
    gtk_box_pack_start (GTK_BOX (vbox), button, FALSE, FALSE, 1);
    gtk_signal_connect(GTK_OBJECT(button), "clicked",
		       GTK_SIGNAL_FUNC(new_module_cb), module_classes[i]);
    gtk_widget_show (button);
  }


  /* Sample List */

  scrolled = gtk_scrolled_window_new (NULL, NULL);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled),
				  GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
  gtk_box_pack_start(GTK_BOX(hbox), scrolled, TRUE, TRUE, 8);
  gtk_widget_show (scrolled);

  clist = gtk_clist_new_with_titles(3, titles);
  gtk_clist_column_titles_passive(GTK_CLIST(clist));
  gtk_clist_set_selection_mode(GTK_CLIST(clist), GTK_SELECTION_BROWSE);
  /* gtk_clist_set_policy(GTK_CLIST(clist), GTK_POLICY_AUTOMATIC,
     GTK_POLICY_AUTOMATIC); */
  /* gtk_clist_set_border(GTK_CLIST(clist), GTK_SHADOW_IN); */
  gtk_clist_set_column_width(GTK_CLIST(clist), 0, 12);
  gtk_clist_set_column_width(GTK_CLIST(clist), 1, 80);
  gtk_clist_set_column_width(GTK_CLIST(clist), 2, 30);
  gtk_clist_set_column_justification(GTK_CLIST(clist), 2,
				     GTK_JUSTIFY_RIGHT);

  gtk_container_add (GTK_CONTAINER (scrolled), clist);

  aube_refresh_samplelist(NULL, (gpointer)clist);
  gtk_widget_show(clist);

  /*  aube_samplelist->handler_id =*/
    gtk_signal_connect(GTK_OBJECT(master),
		       "modules_changed",
		       GTK_SIGNAL_FUNC(aube_refresh_samplelist),
		       clist);
  gtk_signal_connect(GTK_OBJECT(clist), "button_press_event",
		     GTK_SIGNAL_FUNC(aube_samplelist_events), clist);

  /*  aube_samplelist->commands_menu = gtk_menu_new();*/

  gtk_widget_set_usize(GTK_WIDGET(clist), 160, 240);

  commands_menu = (GtkMenu *)gtk_menu_new ();

  menuitem = gtk_menu_item_new_with_label("Save...");
  gtk_menu_append(GTK_MENU(commands_menu), menuitem);
  gtk_signal_connect(GTK_OBJECT(menuitem), "activate",
		     GTK_SIGNAL_FUNC(sample_save_cb), NULL);
  gtk_widget_show(menuitem);

  menuitem = gtk_menu_item_new_with_label("Close");
  gtk_menu_append(GTK_MENU(commands_menu), menuitem);
  gtk_signal_connect(GTK_OBJECT(menuitem), "activate",
		     GTK_SIGNAL_FUNC(aube_remove_sample_cb), clist);
  gtk_widget_show(menuitem);

}

GtkWidget *
master_new(void)
{
  GtkWidget *m;

  m = GTK_WIDGET(gtk_type_new(master_get_type()));
  master_daddy = m;
  return (m);
}

void
master_dismiss(GtkWidget * widget, gpointer data)
{
  gtk_widget_destroy(GTK_WIDGET(data));
}


void
quit_cb(GtkWidget * widget, gpointer data)
{
  gtk_widget_destroy(GTK_WIDGET(data));
  gtk_exit(0);
}

gint
delete_event(GtkWidget * widget, gpointer data)
{
  g_print("delete event occurred\n");
  /*
     if you return TRUE in the "delete_event" signal handler,
     * GTK will emit the "destroy" signal. Returning FALSE means
     * you don't want the window to be destroyed.
     * This is useful for popping up "Are you sure you want to quit?"
     * type dialogs 
   */

  /*
     Change FALSE to TRUE and the main window will be destroyed with
     * a "delete_event" 
   */

  return (FALSE);
}

void
destroy(GtkWidget * widget, gpointer data)
{
  gtk_main_quit();
}

void
master_ack_channels_modified(void)
{
  gtk_signal_emit(GTK_OBJECT(master_daddy), master_signals[UNITS_CHANGED_SIGNAL]);
}

void
new_module_cb(GtkWidget * widget, gpointer data)
{
  module *m;
  module_class *mc;

  mc = (module_class *) data;
  m = aube_module_new_instance(mc);
  aube_module_add_if(m);

#if 0
  if (ui->class_init != NULL)
    ui->class_init();
  u = ui->new_module();
  w = ui->new_if(u);
  u->has_if = w;
  u->class = ui;
  gtk_widget_show(w);
  gtk_signal_emit(GTK_OBJECT(master_daddy), master_signals[UNITS_CHANGED_SIGNAL]);
#endif
}

void
module_config_cb(GtkWidget * widget, gpointer data)
{
  module *u;
  module_class *ui;

  u = (module *) data;
  ui = u->class;
  ui->config_module(u);
}

void
module_clone_cb(GtkWidget * widget, gpointer data)
{
  GtkWidget *w;
  module *ou, *u;
  module_class *ui;

  ou = (module *) data;
  ui = ou->class;

  if (ui->clone_module != NULL) {
    u = ui->clone_module(ou);
    w = ui->new_if(u);
    u->has_if = w;
    u->class = ui;
    gtk_widget_show(w);
    gtk_signal_emit(GTK_OBJECT(master_daddy), master_signals[UNITS_CHANGED_SIGNAL]);
  } else {
    aube_error(AUBE_MESSAGE, "This module cannot be cloned.");
  }
}

void
module_replace_cb(GtkWidget * widget, gpointer data)
{
  module *ou, *u;
  module_class *ui;
  int i, j, k, l = 0;

  ou = ((modulepack *) data)->this_module;
  u = ((modulepack *) data)->selected_module;
  ui = ou->class;

  for (i = 0; i < aube_daddy->nr_modules; i++) {
    for (j = 0; j < aube_daddy->modules[i]->nr_inputs; i++) {
      for (k = 0; k < ou->nr_outputs; k++) {
	if (aube_daddy->modules[i]->inputs[j]->channel == ou->outputs[k]) {
	  aube_daddy->modules[i]->inputs[j]->channel = u->outputs[k];
	  l++;
	}
      }
    }
  }
  if (l > 0)
    gtk_signal_emit(GTK_OBJECT(master_daddy),
		    master_signals[UNITS_CHANGED_SIGNAL]);
}

void
change_unhide_menu_cb(GtkWidget * widget, gpointer data)
{
  GtkWidget *menu;

  menu = modulemenu_new(NULL, (void *) include_unhidden, (void *) select_unhidden);
  gtk_menu_item_remove_submenu(GTK_MENU_ITEM(data));
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(data), menu);
}

void
change_replace_menu_cb(GtkWidget * widget, gpointer data)
{
  GtkWidget *menu;

  menu = modulemenu_new(NULL, (void *) include_unhidden, (void *) select_unhidden);
  menu = modulemenu_new((module *) data, (void *) aube_module_cmp_type, (void *) module_replace_cb);
  gtk_menu_item_remove_submenu(GTK_MENU_ITEM(((module *) data)->class->replace_menuitem));
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(((module *) data)->class->replace_menuitem), menu);
}

int
include_unhidden(module * this_module, module * selected_module)
{
  return (selected_module->has_if == NULL);
}

void
select_unhidden(GtkWidget * widget, gpointer data)
{
  GtkWidget *w;
  module *u;
  module_class *ui;

  u = ((modulepack *) data)->selected_module;
  ui = u->class;

  w = ui->new_if(u);
  u->has_if = w;
  gtk_widget_show(w);
  gtk_signal_emit(GTK_OBJECT(master_daddy), master_signals[UNITS_CHANGED_SIGNAL]);
}

void
clear_workspace_cb(GtkWidget * widget, gpointer data)
{
  gint i, j, n;
  module_class *mc;

#if 0
  /*
     There is a subtle interaction here as we remove modules. As each module
     is removed, aube_daddy->nr_modules is decreased, hence we cannot
     check against it. Also, the remainder of the array is shifted to remain
     continuous. Hence, we play with i,j,n.

     Sure we could do it more easily in reverse, but that would take away
     all the fun :)
   */

  n = aube_daddy->nr_modules;
  j = 0;

  for (i = 0; i < n; i++) {
    if ((mc=aube_daddy->modules[j]->class) && strcmp(mc->type_label, "none")) {
      if (aube_daddy->modules[j]->has_if) {
	aube_daddy->modules[j]->class->close_if
	  (NULL, aube_daddy->modules[j]->has_if);
      } else {
	aube_remove_module(aube_daddy->modules[j]);
      }
    } else {
      j++;
    }
  }
  for(i = 0; i < nr_module_classes; i++) {
    module_classes[i]->instances = 0;
  }
#endif

  n = aube_daddy->nr_modules;
  j = 0;

  for (j = n-1; j >= 0; j--) {
    mc = aube_daddy->modules[j]->class;
    if (mc && strcmp(mc->type_label, "none")) {
      if (aube_daddy->modules[j]->has_if) {
	/* XXX: can remove close_if, just send destroy signal instead...
	aube_daddy->modules[j]->class->close_if
	  (NULL, aube_daddy->modules[j]->has_if);
	*/
	gtk_widget_destroy (aube_daddy->modules[j]->has_if);
      } else {
	aube_remove_module(aube_daddy->modules[j]);
      }
    }
  }
  for(i = 0; i < nr_module_classes; i++) {
    module_classes[i]->instances = 0;
  }

}
