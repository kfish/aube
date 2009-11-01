
#ifndef __SLIDER_H__
#define __SLIDER_H__

#include <gdk/gdk.h>
#include <gtk/gtkvbox.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SLIDER(obj)          GTK_CHECK_CAST (obj, slider_get_type (), Slider)
#define SLIDER_CLASS(klass)  GTK_CHECK_CLASS_CAST (klass, slider_get_type (), SliderClass)
#define IS_SLIDER(obj)       GTK_CHECK_TYPE (obj, slider_get_type ())

	enum {
		SLIDER_TYPE_INT = 100,
		SLIDER_TYPE_FLOAT
	};

	typedef struct _Slider Slider;
	typedef struct _SliderClass SliderClass;

	struct _Slider {
		GtkVBox vbox;
		gchar value_str[16];
		GtkWidget *value_label;
		GtkObject *adj;

		int type;

		union {
			int *iptr;
			float *fptr;
		} data;

		GtkScale *scale;
	};

	struct _SliderClass {
		GtkWindowClass parent_class;

		void (*slider) (Slider * a);
	};

	GType slider_get_type(void);
	GtkWidget *slider_int_new(char *label_str, int *data, int lower,
				  int upper, int vert);
	GtkWidget *slider_float_new(char *label_str, float *data,
				    float lower, float upper, int vert);
	void slider_refresh(GtkWidget * slider);

#ifdef __cplusplus
}
#endif
#endif				/* __SLIDER_H__ */
