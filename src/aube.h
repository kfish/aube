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

#ifndef __AUBE_H__
#define __AUBE_H__

#include "aube_error.h"

#include <gtk/gtk.h>		/* DOH!! */

typedef unsigned char bit8;
typedef short bit16;
typedef int bit32;
typedef long bit64;

#define PLAYBACK_FREQ 44100
#define PLAYBACK_BITRATE 16
#if 1
#define DEFAULT_TICK (1<<9)
#else
#define DEFAULT_TICK 137
#endif

/*
   Some limiting constants ... 
 */
#define MAX_UNITS 256
#define MAX_CHANNELS 8
#define MAX_SAMPLES 256
#define LABEL_LEN 32

#define MAX_TYPES 16
#define TYPE_LABEL_LEN 16

/*
   Module processing return values 
 */
#define PROC_SUCCESS     0
#define PROC_INCOMPLETE  1
#define PROC_FAILURE    -1

/*
   Error reporting levels 
 */
#define AUBE_MESSAGE              0
#define AUBE_ERROR_RECOVERABLE    1
#define AUBE_ERROR_URGENT         2
#define AUBE_ERROR_FATAL          99


/*
   Module types 
 */
enum {
	CONTROLLER,
	SEQUENCER,
	GENERATOR,
	FILTER,
	EFFECT,
	MIXER,
	OUTPUT,
	LAST_UNIT_TYPE
};

typedef struct _aube_type aube_type;
typedef struct _module module;
typedef struct _channel channel;
typedef struct _ch_parameters ch_parameters;
typedef struct _input input;
typedef struct _aube_data aube_data;
typedef struct _seq_channel seq_channel;
typedef struct _module_class module_class;
#if 0
typedef struct _aube_sample aube_sample;
#endif

#if 0
struct _aube_type {
	int type;
	int instances;
	char *type_label[TYPE_LABEL_LEN];
};
#endif

struct _aube_data {
	int nr_modules;
	module *modules[MAX_UNITS];
	int nr_channels;
#if 0
	int nr_types;
	aube_type types[MAX_TYPES];
#endif
	channel *samples[MAX_SAMPLES];
	int nr_samples;
};

#define CH_TYPE_SEQUENCE      0
#define CH_TYPE_AUDIO_STREAM  1
#define CH_TYPE_AUDIO_SAMPLE  2

struct _ch_parameters {
	int ch_type;
	union {
		int sequence_data;
		struct {
			int nr_channels;
			int bitrate;
			int frequency;
		} audio_stream_data;
		struct {
			int s_length;
		} audio_sample_data;
	} d;
};

struct _channel {
	ch_parameters parms;
	module *module;
	char u_label[LABEL_LEN];
	void *data;
};

struct _seq_channel {
	int trigger;
	int vol;
	int pan;
	int accent;
	double pitch;
};

/*
 * An input is simply a pointer to a channel, and
 * an associated type.
 */
struct _input {
	int ch_type;
	channel *channel;
};

struct _module {
	module_class *class;
	/*int type; */
	int on;
	void *has_if;		/*
				   XXX: pointer to widget 
				 */
	char u_label[LABEL_LEN];
	int last_tick;		/*
				   last tick calculated 
				 */
	int (*do_tick) (void *i);
	int nr_inputs;
	input *inputs[MAX_CHANNELS];
	int nr_outputs;
	channel *outputs[MAX_CHANNELS];
};

struct _module_class {
	int category;
	int instances;
	char *type_label;
	char *u_name;
	void (*class_init) ();
	module *(*new_module) ();
	module *(*clone_module) ();
	int (*module_off) ();
	int (*module_on) ();
	int (*config_module) ();
	GtkWidget *(*new_if) ();
	GtkWidget *replace_menuitem;
};

aube_data *aube_data_new();
module_class *aube_get_module_class(char *class_label);
int aube_module_add_if(module * m);
module *aube_module_new_instance(module_class * mc);
int aube_add_module(module * module);
int aube_remove_module(module * module);
int aube_add_sample(channel * sample);
int aube_remove_sample(channel * sample);
input *new_input(int ch_type);
input *clone_input(input * in0);
channel *new_output(int ch_type, module * m, char *label, void *data);
channel *clone_output(channel * out0, module * m, void *data);
int aube_module_clear_outputs(module * u);
int aube_module_toggle(module * u);
int aube_module_set_on(module * m, int on);
/*
   int aube_module_add_if(module *u);
 */
int aube_module_remove_if(module * u);
int aube_module_cmp_type(module * u1, module * u2);

#endif /* __AUBE_H__  */
