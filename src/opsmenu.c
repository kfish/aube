#include <strings.h>
#include <gtk/gtk.h>
#include "opsmenu.h"
#include "master.h"

extern GtkWidget *master_daddy;
extern aube_data *aube_daddy;

extern bit16 zero_buffer[];

guint opsmenu_get_type(void);
static void opsmenu_class_init(OpsmenuClass * class);
static void opsmenu_init(Opsmenu * b);
GtkWidget *opsmenu_new();
void opsmenu_dismiss(GtkWidget * widget, gpointer data);
void reread_inputs_cb(GtkWidget * widget, gpointer data);
void select_input_cb(GtkWidget * widget, gpointer data);

guint
opsmenu_get_type()
{
  static guint b_type = 0;

  if (!b_type) {
    GtkTypeInfo b_info =
    {
      "Opsmenu",
      sizeof(Opsmenu),
      sizeof(OpsmenuClass),
      (GtkClassInitFunc) opsmenu_class_init,
      (GtkObjectInitFunc) opsmenu_init,
      (GtkArgSetFunc) NULL,
      (GtkArgGetFunc) NULL,
    };

    b_type = gtk_type_unique(gtk_menu_bar_get_type(), &b_info);
  }
  return b_type;
}

enum {
  LAST_SIGNAL
};

static guint opsmenu_signals[LAST_SIGNAL+1] =
{0};

static void
opsmenu_class_init(OpsmenuClass * class)
{
  GtkObjectClass *object_class;

  object_class = (GtkObjectClass *) class;

  gtk_object_class_add_signals(object_class, opsmenu_signals, LAST_SIGNAL);

  class->opsmenu = NULL;
}

static void
opsmenu_init(Opsmenu * a)
{
}

GtkWidget *
opsmenu_new(module * module, GtkWidget * parent_if, void *if_hide_cb, void *if_close_cb)
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
  gtk_signal_connect(GTK_OBJECT(menuitem), "activate",
		     GTK_SIGNAL_FUNC(module_config_cb), module);
  gtk_widget_show(menuitem);
#endif

  menuitem = gtk_menu_item_new_with_label("Hide");
  gtk_menu_append(GTK_MENU(menu), menuitem);
  gtk_signal_connect(GTK_OBJECT(menuitem), "activate",
		     GTK_SIGNAL_FUNC(if_hide_cb), parent_if);
  gtk_widget_show(menuitem);

  menuitem = gtk_menu_item_new_with_label("Clone");
  gtk_menu_append(GTK_MENU(menu), menuitem);
  gtk_signal_connect(GTK_OBJECT(menuitem), "activate",
		     GTK_SIGNAL_FUNC(module_clone_cb), module);
  gtk_widget_show(menuitem);

#if 0
  if (module->class) {
    module->class->replace_menuitem = gtk_menu_item_new_with_label("Replace with");

    menu2 = modulemenu_new(module, (void *) aube_module_cmp_type, (void *) module_replace_cb);
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(menuitem), menu2);

    gtk_menu_append(GTK_MENU(menu), menuitem);
    gtk_signal_connect(GTK_OBJECT(master_daddy), "modules_changed",
		       GTK_SIGNAL_FUNC(change_replace_menu_cb), module);
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

void
opsmenu_dismiss(GtkWidget * widget, gpointer data)
{
  gtk_widget_destroy(GTK_WIDGET(data));
}
