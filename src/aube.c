#include <config.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <X11/Xlib.h>
#include <glib.h>
#include <gtk/gtk.h>

#include "aube.h"
#include "master.h"
#include "sample.h"
#include "none.h"

GtkWidget *master_daddy = NULL;
aube_data *aube_daddy = NULL;

extern module_class *module_classes[];
extern int nr_module_classes;

extern gint master_signals[];

int tick = DEFAULT_TICK;
int beat;
int current_tick = 0;

int bpm = 142;
int beat_index = 0;

int module_do_tick(module * module);
int aube_get_type(aube_data * a, char *typename);


/*
 * module_do_tick(module *m)
 *
 * process a particular module by calling module_do_tick for each of
 * its inputs. Once all the inputs are calculated, we call this module's
 * do_tick function.
 *
 * If any module lower down the tree returned PROC_INCOMPLETE, we do not
 * call our do_tick function, and we also return PROC_INCOMPLETE.
 *
 * This mechanism allows for as many modules as possible to be calculated
 * while maintaining dependencies.
 */
int module_do_tick(module * u)
{
	int i, failure = 0;
	module *u_tmp;

	for (i = 0; i < u->nr_inputs; i++) {
		if (u->inputs[i] && u->inputs[i]->channel) {
			u_tmp = u->inputs[i]->channel->module;
			if ((u_tmp != NULL) && (u_tmp->on)
			    && (u_tmp->last_tick < current_tick)) {
				if (module_do_tick(u_tmp) != PROC_SUCCESS) {
					failure = 1;
				}
			}
		}
	}

	if (!failure) {
		if (u->do_tick(u) == PROC_SUCCESS) {
			u->last_tick = current_tick;
			return PROC_SUCCESS;
		}
	}
	return PROC_INCOMPLETE;
}


/*
 * aube_do_tick()
 *
 * This is the main processing function. From here, modules are calculated
 * recursively down the processing tree.
 */
gint aube_do_tick(gpointer data)
{
	int i, failure = 0;
	int nr_outputs = 0;

	if (bpm < 60)
		bpm = 60;
	beat = (30 * PLAYBACK_FREQ) / bpm;	/*
						   samples per beat 
						 */

	beat_index %= beat;

	/*
	 * We call module_do_tick for each output module.
	 *
	 * For now, we define output modules as those with no output channels of
	 * their own. We really should be able to define it more finely, eg. the
	 * sample recorder should always record its input, even if its output isn't
	 * connected to anything. With the current scheme, unless its output
	 * is connected to something which eventually leads to an "output module",
	 * the sample recorder won't be calculated.
	 */
	for (i = 0; i < ((aube_data *) data)->nr_modules; i++) {
		if ((((aube_data *) data)->modules[i]->nr_outputs == 0) &&
		    ((aube_data *) data)->modules[i]->on) {

			nr_outputs++;

			if (module_do_tick
			    (((aube_data *) data)->modules[i]) !=
			    PROC_SUCCESS) {
				failure = 1;
			}
		}
	}

	/*
	   If the processing failed for any of the output modules, we don't
	   * increment current_tick, hence next time we are called we try to
	   * process this tick again, finishing off incomplete modules
	 */
	if (nr_outputs > 0 && !failure) {
		beat_index += tick;
		current_tick++;
	} else {
		usleep(50);
	}

	return 1;
}


/*
   This is really bad because it shouldn't be in the back end 
 */
enum {
	UNITS_CHANGED_SIGNAL,
	LAST_SIGNAL
};


input *new_input(int ch_type)
{
	input *in;

	if ((in = malloc(sizeof(input)))) {
		in->ch_type = ch_type;
		in->channel = NULL;
	}
	return in;		/*
				   return NULL if malloc failed 
				 */
}

input *clone_input(input * in0)
{
	input *in;

	if ((in = malloc(sizeof(input)))) {
		in->ch_type = in0->ch_type;
		in->channel = in0->channel;
	}
	return in;
}

channel *new_output(int ch_type, module * m, char *label, void *data)
{
	channel *out;

	if ((out = malloc(sizeof(channel)))) {
		out->parms.ch_type = ch_type;
		out->module = m;
		snprintf(out->u_label, sizeof(out->u_label), "%s", label);
		out->data = data;
	}
	return out;
}

channel *clone_output(channel * out0, module * m, void *data)
{
	return new_output(out0->parms.ch_type, m, out0->module->u_label,
			  data);
}

int aube_module_clear_outputs(module * u)
{
	int i;

	for (i = 0; i < u->nr_outputs; i++) {
		switch (u->outputs[i]->parms.ch_type) {
		case CH_TYPE_AUDIO_STREAM:
			memset(u->outputs[i]->data, 0, tick * 2);
			break;
		case CH_TYPE_SEQUENCE:
			((seq_channel *) u->outputs[i]->data)->trigger = 0;
			((seq_channel *) u->outputs[i]->data)->vol = 0;
			((seq_channel *) u->outputs[i]->data)->pan = 32;
			((seq_channel *) u->outputs[i]->data)->pitch = 0;
			break;
		default:
			break;
		}
	}
	return 1;
}

module *aube_module_new_instance(module_class * mc)
{
	module *m;

	if (mc->class_init)
		mc->class_init();
	m = mc->new_module();
	m->class = mc;

	return m;
}

int aube_module_set_on(module * m, int on)
{
	m->on = on;
	if (m->on) {
		if (m->class->module_on)
			m->class->module_on(m);
	} else {
		if (m->class->module_off)
			m->class->module_off(m);
		aube_module_clear_outputs(m);
	}

	return 1;
}

int aube_module_toggle(module * m)
{
	return aube_module_set_on(m, 1 - m->on);
}

int aube_module_add_if(module * m)
{
	m->has_if = m->class->new_if(m);
	gtk_widget_show(m->has_if);
	g_signal_emit(G_OBJECT(master_daddy),
		      master_signals[UNITS_CHANGED_SIGNAL], 0);
	return 1;
}

int aube_module_remove_if(module * u)
{
	u->has_if = NULL;
	master_ack_channels_modified();
#if 0
	g_signal_emit(G_OBJECT(master_daddy),
		      master_signals[UNITS_CHANGED_SIGNAL], 0);
#endif
	return 1;
}

int aube_module_cmp_type(module * u1, module * u2)
{
	return (u1->class == u2->class);
}


aube_data *aube_data_new()
{
	aube_data *a;

	a = (aube_data *) g_malloc0(sizeof(aube_data));
	a->nr_modules = 0;
	a->nr_channels = 0;
	a->nr_samples = 0;

	return a;
}


int aube_add_module(module * module)
{
	char temp[sizeof(module->u_label)];
	aube_daddy->modules[aube_daddy->nr_modules] = module;
	aube_daddy->nr_modules++;
	aube_daddy->nr_channels += module->nr_outputs;

	/*module->type = aube_get_type(aube_daddy, module->u_label); */
	/*aube_daddy->types[module->type].instances++; */
	module->class->instances++;

	/* If we're using snprintf, we need to store the existing string first. */
	snprintf(temp, sizeof(temp), "%s", module->u_label);
	sprintf(module->u_label, "%s-%d", temp, module->class->instances);
/*
	  aube_daddy->types[module->type].instances);
*/

	module->last_tick = 0;

	/*
	   g_signal_emit(G_OBJECT(master_daddy), master_signals[UNITS_CHANGED_SIGNAL], 0);
	 */

	return 1;
}

int aube_remove_module(module * module)
{
	int i;

	for (i = 0;
	     i < aube_daddy->nr_modules
	     && aube_daddy->modules[i] != module; i++);
	if (i != aube_daddy->nr_modules) {
		aube_daddy->nr_modules--;
		for (; i < (aube_daddy->nr_modules); i++) {
			aube_daddy->modules[i] =
			    aube_daddy->modules[i + 1];
		}
	}
	aube_daddy->nr_channels -= module->nr_outputs;

	master_ack_channels_modified();
#if 0
	g_signal_emit(G_OBJECT(master_daddy),
		      master_signals[UNITS_CHANGED_SIGNAL], 0);
#endif

	return 1;
}

int aube_add_sample(channel * sample)
{
	aube_daddy->samples[aube_daddy->nr_samples] = sample;
	aube_daddy->nr_samples++;

	master_ack_channels_modified();
#if 0
	g_signal_emit(G_OBJECT(master_daddy),
		      master_signals[UNITS_CHANGED_SIGNAL], 0);
#endif

	return 1;
}

int aube_remove_sample(channel * sample)
{
	int i;

	for (i = 0;
	     i < aube_daddy->nr_samples
	     && aube_daddy->samples[i] != sample; i++);
	if (i != aube_daddy->nr_samples) {
		aube_daddy->nr_samples--;
		for (; i < (aube_daddy->nr_samples); i++) {
			aube_daddy->samples[i] =
			    aube_daddy->samples[i + 1];
		}
	}
	master_ack_channels_modified();
#if 0
	g_signal_emit(G_OBJECT(master_daddy),
		      master_signals[UNITS_CHANGED_SIGNAL], 0);
#endif

	return 1;
}

module_class *aube_get_module_class(char *class_label)
{
	int i;

	for (i = 0; i < nr_module_classes; i++) {
		if (!strcmp(class_label, module_classes[i]->type_label)) {
			return module_classes[i];
		}
	}
	return NULL;
}

#if 0
int aube_get_type(aube_data * a, char *typename)
{
	int i;

	for (i = 0; i < a->nr_types; i++) {
		if (!strncmp
		    ((char *) a->types[i].type_label, typename,
		     TYPE_LABEL_LEN)) {
			return i;
		}
	}
	snprintf((char *) a->types[i].type_label,
		 sizeof(a->types[i].type_label), "%s", typename);
	a->types[i].type = i;
	a->types[i].instances = 0;
	a->nr_types++;
	return i;
}
#endif


int main(int argc, char **argv)
{
	int show_version, show_help;
	none *nn;
	GtkWidget *master;
	char buf[1024];
	int i;

	gtk_init(&argc, &argv);
	snprintf(buf, sizeof(buf), "%s/gtkrc", DATADIR);
	gtk_rc_parse(buf);

	srandom(time(NULL));

	show_version = FALSE;
	show_help = FALSE;

	for (i = 1; i < argc; i++) {
		if ((strcmp(argv[i], "--help") == 0) ||
		    (strcmp(argv[i], "-h") == 0)) {
			show_help = TRUE;
		} else if ((strcmp(argv[i], "--version") == 0) ||
			   (strcmp(argv[i], "-v") == 0)) {
			show_version = TRUE;
		}
		/*
		 * anything else starting with a '-' is an error.
		 */
		else if (argv[i][0] == '-') {
			show_help = TRUE;
		}
	}

	if (show_version) {
		g_print("AUBE version " VERSION "\n");
		g_print("Copyright (C) 1998,1999 Conrad Parker\n");
		g_print("AUBE comes with ABSOLUTELY NO WARRANTY.\n");
		g_print("You may redistribute copies of AUBE\n");
		g_print
		    ("under the terms of the GNU General Public License.\n");
		g_print
		    ("For more information about these matters, see the files named COPYING\n");
	}
	if (show_help) {
		g_print("Usage: %s [option ...] [files ...]\n", argv[0]);
		g_print("Valid options are:\n");
		g_print("  -h --help              Output this help.\n");
		g_print("  -v --version           Output version info.\n");
		g_print
		    ("  --display <display>    Use the designated X display.\n");
		g_print
		    ("\nFor more details see http://www.cse.unsw.edu.au/~conradp/aube/\n");
	}
	if (show_version || show_help)
		exit(0);


	master = master_new();
	gtk_idle_add((GtkFunction) (aube_do_tick), aube_daddy);
	gtk_widget_show(master);
	nn = none_new();

	for (i = 1; i < argc; i++) {
		if (argv[i][0] != '-')
			sample_load(argv[i]);
	}

	gtk_main();

	return 0;
}
