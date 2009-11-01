#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <X11/Xlib.h>
#include <glib.h>
#include <gtk/gtk.h>

#include "whitenoise.h"
#include "master.h"

#include "whitenoise_if.h"
#include "none_config.h"

extern int bpm, beat;
extern int tick;

int whitenoise_calc_note(whitenoise * wn);

module_class whitenoise_mif = {
	GENERATOR,
	0,
	(char *) "w-noise",
	(char *) "Whitenoise",
	NULL,
	(void *) whitenoise_new,
	(void *) whitenoise_clone,
	NULL,
	NULL,
	(void *) none_config_cb,
	(void *) whitenoise_if_new,
	(void *) whitenoise_if_close_cb
};

whitenoise *whitenoise_new()
{
	whitenoise *wn;

	wn = (whitenoise *) malloc(sizeof(whitenoise));
	wn->module.class = &whitenoise_mif;
	wn->module.on = 0;
	wn->module.nr_inputs = 0;
	wn->module.nr_outputs = 1;
	wn->module.outputs[0] = malloc(sizeof(channel));
	wn->module.outputs[0]->parms.ch_type = CH_TYPE_AUDIO_STREAM;
	wn->module.outputs[0]->data = (bit16 *) (&(wn->tick_buffer));

	/*
	   memset(wn->module.outputs[0]->tick_buffer, 0, sizeof(wn->module.outputs[0]->tick_buffer));
	 */
	aube_module_clear_outputs((module *) wn);
	snprintf(wn->module.u_label, sizeof(wn->module.u_label),
		 "w-noise");
	snprintf(wn->module.outputs[0]->u_label,
		 sizeof(wn->module.outputs[0]->u_label), "out");
	wn->module.outputs[0]->module = (module *) wn;

	wn->module.do_tick = (void *) whitenoise_calc_note;

	aube_add_module((module *) wn);

	wn->vol = 8;
	wn->pan = 16;

	return wn;
}

whitenoise *whitenoise_clone(whitenoise * own)
{
	whitenoise *wn;

	wn = (whitenoise *) malloc(sizeof(whitenoise));
	memcpy(wn, own, sizeof(whitenoise));

	wn->module.nr_inputs = 0;
	wn->module.nr_outputs = 1;
	wn->module.outputs[0] = malloc(sizeof(channel));
	wn->module.outputs[0]->parms.ch_type = CH_TYPE_AUDIO_STREAM;
	wn->module.outputs[0]->data = (bit16 *) (&(wn->tick_buffer));

	aube_module_clear_outputs((module *) wn);
	snprintf(wn->module.u_label, sizeof(wn->module.u_label),
		 "w-noise");
	snprintf(wn->module.outputs[0]->u_label,
		 sizeof(wn->module.outputs[0]->u_label), "out");
	wn->module.outputs[0]->module = (module *) wn;

	wn->module.do_tick = (void *) whitenoise_calc_note;

	aube_add_module((module *) wn);

	return wn;
}

int whitenoise_calc_note(whitenoise * wn)
{
	unsigned int i;
	int lpan, rpan;

	if (wn->pan <= 16) {
		lpan = wn->pan;
		rpan = 16;
	} else {
		lpan = 16;
		rpan = 31 - wn->pan;
	}

	for (i = 0; i < tick; i += 2) {
		wn->tick_buffer[i] =
		    (bit16) ((((random() % (1 << 16)) -
			       (1 << 15)) * wn->vol) >> 6);
		wn->tick_buffer[i + 1] =
		    (bit16) ((wn->tick_buffer[i] * rpan) >> 4);
		wn->tick_buffer[i] =
		    (bit16) ((wn->tick_buffer[i] * lpan) >> 4);
	}

	return PROC_SUCCESS;
}
