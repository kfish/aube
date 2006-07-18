#include <stdio.h>
#include <strings.h>
#include <gtk/gtk.h>
#include "slider.h"


guint slider_get_type(void);
static void slider_class_init(SliderClass * class);
static void slider_init(Slider * b);
GtkWidget *slider_int_new();
void slider_dismiss(GtkWidget * widget, gpointer data);
void slider_cb(GtkWidget * widget, gpointer data);

guint
slider_get_type(void)
{
  static guint b_type = 0;

  if (!b_type) {
    GtkTypeInfo b_info =
    {
      "Slider",
      sizeof(Slider),
      sizeof(SliderClass),
      (GtkClassInitFunc) slider_class_init,
      (GtkObjectInitFunc) slider_init,
      (GtkArgSetFunc) NULL,
      (GtkArgGetFunc) NULL,
    };

    b_type = gtk_type_unique(gtk_vbox_get_type(), &b_info);
  }
  return b_type;
}

enum {
  LAST_SIGNAL
};

static guint slider_signals[LAST_SIGNAL+1] =
{0};

static void
slider_class_init(SliderClass * class)
{
  GtkObjectClass *object_class;

  object_class = (GtkObjectClass *) class;

  gtk_object_class_add_signals(object_class, slider_signals, LAST_SIGNAL);

  class->slider = NULL;
}

static void
slider_init(Slider * a)
{
}

GtkWidget *
slider_int_new(char *label_str, int *data, int lower, int upper, int vert)
{
  Slider *slider;
  GtkWidget *label;

  slider = gtk_type_new(slider_get_type());

  gtk_container_border_width(GTK_CONTAINER(slider), 1);

  slider->type = SLIDER_TYPE_INT;
  slider->data.iptr = data;

  sprintf (slider->value_str, "%d", *data);
  slider->value_label = gtk_label_new (slider->value_str);
  gtk_box_pack_start (GTK_BOX (slider), slider->value_label, FALSE, TRUE, 0);
  gtk_widget_show (slider->value_label);

  slider->adj = gtk_adjustment_new((gfloat) (lower + upper - *data),
				   (gfloat) lower, (gfloat) upper,
				   1.0, 1.0, 0.0);

  if (vert) {
    slider->scale = GTK_SCALE(gtk_vscale_new(GTK_ADJUSTMENT(slider->adj)));
    /* gtk_widget_set_usize(GTK_WIDGET(slider->scale), 16, 120); */
  } else {
    slider->scale = GTK_SCALE(gtk_hscale_new(GTK_ADJUSTMENT(slider->adj)));
    /* gtk_widget_set_usize(GTK_WIDGET(slider->scale), 120, 16); */
  }
  gtk_box_pack_start(GTK_BOX(slider), GTK_WIDGET(slider->scale), TRUE, TRUE, 0);
  gtk_range_set_update_policy(GTK_RANGE(slider->scale), GTK_UPDATE_CONTINUOUS);

  /* draw number above scale */
  gtk_scale_set_draw_value(GTK_SCALE(slider->scale), FALSE);

  gtk_signal_connect(GTK_OBJECT(slider->adj), "value_changed",
		     GTK_SIGNAL_FUNC(slider_cb), slider);
  gtk_widget_show(GTK_WIDGET(slider->scale));

  label = gtk_label_new(label_str);
  gtk_box_pack_start(GTK_BOX(slider), label,
		     FALSE, FALSE, 0);
  gtk_widget_show(label);

  return GTK_WIDGET(slider);
}

GtkWidget *
slider_float_new(char *label_str, float *data, float lower, float upper, int vert)
{
  Slider *slider;
  GtkWidget *label;

  slider = gtk_type_new(slider_get_type());

  gtk_container_border_width(GTK_CONTAINER(slider), 1);

  slider->type = SLIDER_TYPE_FLOAT;
  slider->data.fptr = data;

  snprintf (slider->value_str, sizeof (slider->value_str), "%5.2f", *data);
  slider->value_label = gtk_label_new (slider->value_str);
  gtk_box_pack_start (GTK_BOX (slider), slider->value_label, FALSE, TRUE, 0);
  gtk_widget_show (slider->value_label);

  slider->adj = gtk_adjustment_new(lower + upper - *data,
				   lower, upper,
				   1.0, 1.0, 0.0);

  if (vert) {
    slider->scale = GTK_SCALE(gtk_vscale_new(GTK_ADJUSTMENT(slider->adj)));
    /* gtk_widget_set_usize(GTK_WIDGET(slider->scale), 16, 120); */
  } else {
    slider->scale = GTK_SCALE(gtk_hscale_new(GTK_ADJUSTMENT(slider->adj)));
    /* gtk_widget_set_usize(GTK_WIDGET(slider->scale), 120, 16); */
  }
  gtk_box_pack_start(GTK_BOX(slider), GTK_WIDGET(slider->scale), TRUE, TRUE, 0);
  gtk_range_set_update_policy(GTK_RANGE(slider->scale), GTK_UPDATE_CONTINUOUS);

  /* draw number above scale */
  gtk_scale_set_draw_value(GTK_SCALE(slider->scale), FALSE);

  gtk_signal_connect(GTK_OBJECT(slider->adj), "value_changed",
		     GTK_SIGNAL_FUNC(slider_cb), slider);
  gtk_widget_show(GTK_WIDGET(slider->scale));

  label = gtk_label_new(label_str);
  gtk_box_pack_start(GTK_BOX(slider), label,
		     FALSE, FALSE, 0);
  gtk_widget_show(label);

  return GTK_WIDGET(slider);
}

void
slider_dismiss(GtkWidget * widget, gpointer data)
{
  gtk_widget_destroy(GTK_WIDGET(data));
}

void
slider_cb(GtkWidget * widget, gpointer data)
{
  Slider * slider = SLIDER(data);
  GtkAdjustment * adj = (GtkAdjustment *)slider->adj;

  *(slider->data.iptr) =
    (int) (adj->lower + adj->upper - adj->value);

  snprintf (slider->value_str, sizeof (slider->value_str), "%d", *(slider->data.iptr));
  gtk_label_set_text (GTK_LABEL(SLIDER(data)->value_label), slider->value_str);

#if 0
  g_print("slider: %d\n", *(SLIDER(data)->data));
#endif
}

void
slider_float_cb(GtkWidget * widget, gpointer data)
{
  Slider * slider = SLIDER(data);
  GtkAdjustment * adj = (GtkAdjustment *)slider->adj;

  *(slider->data.fptr) = adj->lower + adj->upper - adj->value;

  snprintf (slider->value_str, sizeof (slider->value_str), "%5.2f", *(slider->data.fptr));
  gtk_label_set_text (GTK_LABEL(SLIDER(data)->value_label), slider->value_str);

#if 0
  g_print("slider: %d\n", *(SLIDER(data)->data));
#endif
}

void
slider_refresh(GtkWidget * slider)
{
  switch (SLIDER(slider)->type) {
  case SLIDER_TYPE_INT:
    GTK_ADJUSTMENT(SLIDER(slider)->adj)->value = GTK_ADJUSTMENT(SLIDER(slider)->adj)->upper - *(SLIDER(slider)->data.iptr);
	break;
  case SLIDER_TYPE_FLOAT:
    GTK_ADJUSTMENT(SLIDER(slider)->adj)->value = GTK_ADJUSTMENT(SLIDER(slider)->adj)->upper - *(SLIDER(slider)->data.fptr);
	break;
  default:
    printf ("Ooops : Bad slider type : %d\n", SLIDER(slider)->type);
    return;
  }

  gtk_range_slider_update(GTK_RANGE(SLIDER(slider)->scale));
}
