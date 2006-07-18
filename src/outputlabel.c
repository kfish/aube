#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <gtk/gtk.h>
#include "levelmeter.h"
#include "outputlabel.h"

extern GtkWidget *master_daddy;
extern aube_data *aube_daddy;

extern bit16 zero_buffer[];

static void outputlabel_class_init(OutputlabelClass * klass);
static void outputlabel_init(Outputlabel * b);
GtkWidget *outputlabel_new();
void outputlabel_drag_request(GtkWidget * widget, GdkEvent * event);
void outputlabel_dismiss(GtkWidget * widget, gpointer data);
gint outputlabel_update_levelmeters(gpointer data);

#if 0
GtkWidget *shape_create_icon(char *xpm_file,
			     gint x,
			     gint y,
			     gint px,
			     gint py,
			     gint window_type);
#endif

GType
outputlabel_get_type(void)
{
  static GType b_type = 0;

  if (!b_type) {
    static const GTypeInfo b_info =
    {
      sizeof(OutputlabelClass),
      NULL, /* base_init */
	  NULL, /* base_finalise */
      (GClassInitFunc) outputlabel_class_init ,
	  NULL, /* class_finalize */
	  NULL, /* class_data */
      sizeof(Outputlabel),
	  0, /* n_preallocs */
	  (GInstanceInitFunc) outputlabel_init,
    };

    b_type = g_type_register_static(GTK_TYPE_FRAME,
                                                      "Outputlabel",
	                                                   &b_info, 0);
  }
  return b_type;
}

static void
outputlabel_class_init(OutputlabelClass * klass)
{
  
}

static void
outputlabel_init(Outputlabel * a)
{
}

GtkWidget *
outputlabel_new(module * module, int output_i)
{
  Outputlabel *outputlabel;
  GtkWidget *widget;
  GtkWidget *hbox;

#if 0
  GdkPoint hotspot =
  {5, 5};

  char *possible_drag_types[] =
  {"text/plain"};

  static GtkWidget *drag_icon = NULL;
  static GtkWidget *drop_icon = NULL;

  if (!drag_icon) {
    drag_icon = shape_create_icon("Modeller.xpm",
				  440, 140, 0, 0, GTK_WINDOW_POPUP);

    g_signal_connect(G_OBJECT(drag_icon), "destroy",
		       G_CALLBACK(gtk_widget_destroyed),
		       &drag_icon);

    gtk_widget_hide(drag_icon);
  }
  if (!drop_icon) {
    drop_icon = shape_create_icon("3DRings.xpm",
				  440, 140, 0, 0, GTK_WINDOW_POPUP);

    g_signal_connect(G_OBJECT(drop_icon), "destroy",
		       G_CALLBACK(gtk_widget_destroyed),
		       &drop_icon);

    gtk_widget_hide(drop_icon);
  }
  gdk_dnd_set_drag_shape(drag_icon->window, &hotspot,
			 drop_icon->window, &hotspot);
#endif

  outputlabel = gtk_type_new(outputlabel_get_type());
  gtk_frame_set_shadow_type(GTK_FRAME(outputlabel), GTK_SHADOW_ETCHED_OUT);
  gtk_container_border_width(GTK_CONTAINER(outputlabel), 0);

  outputlabel->module = module;
  outputlabel->output_i = output_i;

  gtk_container_border_width(GTK_CONTAINER(outputlabel), 1);

  hbox = gtk_hbox_new(FALSE, 1);
  gtk_container_add(GTK_CONTAINER(outputlabel), hbox);
  gtk_widget_show(hbox);

  widget = levelmeter_new(6);
  gtk_box_pack_start(GTK_BOX(hbox), widget, FALSE, TRUE, 0);
  gtk_widget_show(widget);
  outputlabel->levelmeter_left = widget;

  widget = levelmeter_new(4);
  gtk_box_pack_start(GTK_BOX(hbox), widget, FALSE, TRUE, 0);
  gtk_widget_show(widget);
  outputlabel->levelmeter_right = widget;

  outputlabel->timeout_tag = gtk_timeout_add(30,
			       (GtkFunction) outputlabel_update_levelmeters,
					     outputlabel);

  widget = gtk_label_new(module->outputs[output_i]->u_label);
  gtk_box_pack_start(GTK_BOX(hbox), widget, FALSE, TRUE, 0);
  gtk_widget_show(widget);

#if 0
  gtk_widget_realize(widget);
  g_signal_connect(G_OBJECT(widget), "drag_request_event",
		     G_CALLBACK(outputlabel_drag_request), widget);
  gtk_widget_dnd_drag_set(widget, TRUE, possible_drag_types, 1);
#endif

  g_signal_connect(G_OBJECT(outputlabel), "destroy",
		     G_CALLBACK(outputlabel_dismiss), outputlabel);

  return GTK_WIDGET(outputlabel);
}

void
outputlabel_drag_request(GtkWidget * widget, GdkEvent * event)
{
#define DND_STRING "Oath"
  /*
     gtk_widget_dnd_data_set(widget, event, DND_STRING, strlen(DND_STRING)+1);
   */
}

void
outputlabel_dismiss(GtkWidget * widget, gpointer data)
{
  gtk_timeout_remove(OUTPUTLABEL(data)->timeout_tag);
  gtk_widget_destroy(GTK_WIDGET(data));
}

typedef struct _cursoroffset {
  gint x, y;
} CursorOffset;


#if 0
GtkWidget *
shape_create_icon(char *xpm_file,
		  gint x,
		  gint y,
		  gint px,
		  gint py,
		  gint window_type)
{
  GtkWidget *window;
  GtkWidget *pixmap;
  GtkWidget *fixed;
  CursorOffset *icon_pos;
  GdkGC *gc;
  GdkBitmap *gdk_pixmap_mask;
  GdkPixmap *gdk_pixmap;
  GtkStyle *style;

  style = gtk_widget_get_default_style();
  gc = style->black_gc;

  /*
   * GDK_WINDOW_TOPLEVEL works also, giving you a title border
   */
  window = gtk_window_new(window_type);

  fixed = gtk_fixed_new();
  gtk_widget_set_usize(fixed, 100, 100);
  gtk_container_add(GTK_CONTAINER(window), fixed);
  gtk_widget_show(fixed);

  gtk_widget_set_events(window,
			gtk_widget_get_events(window) |
			GDK_BUTTON_MOTION_MASK |
			GDK_POINTER_MOTION_HINT_MASK |
			GDK_BUTTON_PRESS_MASK);

  gtk_widget_realize(window);
  gdk_pixmap = gdk_pixmap_create_from_xpm(window->window, &gdk_pixmap_mask,
					  &style->bg[GTK_STATE_NORMAL],
					  xpm_file);

  pixmap = gtk_pixmap_new(gdk_pixmap, gdk_pixmap_mask);
  gtk_fixed_put(GTK_FIXED(fixed), pixmap, px, py);
  gtk_widget_show(pixmap);

  gtk_widget_shape_combine_mask(window, gdk_pixmap_mask, px, py);

  icon_pos = g_new(CursorOffset, 1);
  gtk_object_set_user_data(GTK_OBJECT(window), icon_pos);

  gtk_widget_set_uposition(window, x, y);
  gtk_widget_show(window);

  return window;
}
#endif

int
channel_calc_level(channel * ch, int offset)
{
  gint i;
  long long rtot = 0;

  switch (ch->parms.ch_type) {
  case CH_TYPE_SEQUENCE:
    return 0;
    break;
  case CH_TYPE_AUDIO_STREAM:
    for (i = offset; i < DEFAULT_TICK; i += 2) {
      rtot += abs(((bit16 *) (ch->data))[i]);
    }
    rtot /= (i / 2);
    rtot /= 655;
    if (rtot > 10)
      rtot = 10;
    return ((int) rtot);
    break;
  case CH_TYPE_AUDIO_SAMPLE:
    return 0;
    break;
  default:
    return 0;
    break;
  }
}

gint
outputlabel_update_levelmeters(gpointer data)
{
  Outputlabel *outputlabel;

  outputlabel = OUTPUTLABEL(data);

  if (outputlabel->module->on) {
    levelmeter_set_level(LEVELMETER(outputlabel->levelmeter_left),
			 channel_calc_level(outputlabel->module->outputs[outputlabel->output_i], 0));
    levelmeter_set_level(LEVELMETER(outputlabel->levelmeter_right),
			 channel_calc_level(outputlabel->module->outputs[outputlabel->output_i], 1));
  } else {
    levelmeter_set_level(LEVELMETER(outputlabel->levelmeter_left), 0);
    levelmeter_set_level(LEVELMETER(outputlabel->levelmeter_right), 0);
  }

  return 1;
}
