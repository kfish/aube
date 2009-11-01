#ifndef __SAMPLE_RECORDER_IF_H__
#define __SAMPLE_RECORDER_IF_H__

#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include "modulewindow.h"
#include "sample_recorder.h"

G_BEGIN_DECLS

#define SAMPLE_RECORDER_IF(obj)          GTK_CHECK_CAST (obj, sample_recorder_if_get_type (), SampleRecorderIF)
#define SAMPLE_RECORDER_IF_CLASS(klass)  GTK_CHECK_CLASS_CAST (klass, sample_recorder_if_get_type (), SampleRecorderIFClass)
#define IS_SAMPLE_RECORDER_IF(obj)       GTK_CHECK_TYPE (obj, sample_recorder_if_get_type ())

typedef struct _SampleRecorderIF SampleRecorderIF;
typedef struct _SampleRecorderIFClass SampleRecorderIFClass;

struct _SampleRecorderIF {
	ModuleWindow window;

	GtkWidget *record_next_check, *record_toggle;
	GtkWidget *record_pbar;
	GtkWidget *samplename_entry;
	gint recordfunc_tag;
	sample_recorder *data;
};

struct _SampleRecorderIFClass {
	ModuleWindowClass parent_class;

	void (*sample_recorder_if) (SampleRecorderIF * a);
};

GType sample_recorder_if_get_type(void);
GtkWidget *sample_recorder_if_new(sample_recorder * pn);
void sample_recorder_if_close_cb(GtkWidget * widget,
					 gpointer data);
G_END_DECLS

#endif /* __SAMPLE_RECORDER_IF_H__ */
