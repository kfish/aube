
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <X11/Xlib.h>
#include <glib.h>
#include <gtk/gtk.h>

#include "delay.h"
#include "master.h"

#include "delay_if.h"
#include "none_config.h"

extern int bpm, beat;
extern int tick;

int delay_calc_note(delay * dl);

module_class delay_mif = {
	EFFECT,
	0,
	(char *) "delay",
	(char *) "Delay",
	NULL,
	(void *) delay_new,
	(void *) delay_clone,
	NULL,
	NULL,
	(void *) none_config_cb,
	(void *) delay_if_new,
	(void *) delay_if_close_cb
};

delay *delay_new()
{
	delay *dl;

	dl = (delay *) malloc(sizeof(delay));
	dl->module.class = &delay_mif;
	dl->module.on = 0;
	dl->module.nr_inputs = 1;
	dl->module.inputs[0] = new_input(CH_TYPE_AUDIO_STREAM);
	dl->module.nr_outputs = 1;
	dl->module.outputs[0] =
	    new_output(CH_TYPE_AUDIO_STREAM, (module *) dl, (char *) "out",
		       (&(dl->delay_buffer)));

	aube_module_clear_outputs((module *) dl);
	snprintf(dl->module.u_label, sizeof(dl->module.u_label), "delay");

	dl->module.do_tick = (void *) delay_calc_note;

	aube_add_module((module *) dl);

	dl->offset = 0;
	dl->input_vol = 64;
	dl->delay_vol = 16;
	dl->pan = 16;
	dl->dbi = 0;

	return dl;
}

delay *delay_clone(delay * odl)
{
	delay *dl;

	dl = (delay *) malloc(sizeof(delay));
	memcpy(dl, odl, sizeof(delay));

	dl->module.nr_inputs = 1;
	dl->module.inputs[0] = clone_input(odl->module.inputs[0]);
	dl->module.nr_outputs = 1;
	dl->module.outputs[0] =
	    new_output(CH_TYPE_AUDIO_STREAM, (module *) dl, (char *) "out",
		       (&(dl->delay_buffer)));

	aube_module_clear_outputs((module *) dl);
	snprintf(dl->module.u_label, sizeof(dl->module.u_label), "delay");

	dl->module.do_tick = (void *) delay_calc_note;

	aube_add_module((module *) dl);

	return dl;
}

int delay_calc_note(delay * dl)
{
	unsigned int i;
	int lpan, rpan;
	int delayi;

	if (dl->offset > 1) {
		dl->module.outputs[0]->data = &(dl->delay_buffer[dl->dbi]);
		delayi = dl->dbi + DELAY_BUFFER_LENGTH - (dl->offset) * 2;
		delayi %= DELAY_BUFFER_LENGTH;

		if (dl->pan <= 16) {
			lpan = dl->pan;
			rpan = 16;
		} else {
			lpan = 16;
			rpan = 31 - dl->pan;
		}

		for (i = 0; i < tick; i += 2) {
			dl->delay_buffer[dl->dbi] =
			    (bit16) (((((bit16 *) dl->module.inputs[0]->
					channel->data)[i] *
				       dl->input_vol) >> 6) +
				     ((dl->delay_buffer[delayi] * lpan *
				       dl->delay_vol) >> 10));
			dl->delay_buffer[dl->dbi + 1] =
			    (bit16) (((((bit16 *) dl->module.inputs[0]->
					channel->data)[i +
						       1] *
				       dl->input_vol) >> 6) +
				     ((dl->delay_buffer[delayi + 1] *
				       rpan * dl->delay_vol) >> 10));

			dl->dbi += 2;
			dl->dbi %= DELAY_BUFFER_LENGTH;
			delayi += 2;
			delayi %= DELAY_BUFFER_LENGTH;
		}
	} else {
		memset(&(dl->delay_buffer[dl->dbi]), 0, tick * 2);
		dl->dbi += tick;
		dl->dbi %= DELAY_BUFFER_LENGTH;
		dl->module.outputs[0]->data =
		    dl->module.inputs[0]->channel->data;
	}

	return PROC_SUCCESS;
}
