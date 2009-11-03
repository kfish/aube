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

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <X11/Xlib.h>
#include <glib.h>
#include <gtk/gtk.h>

#include "master.h"
#include "drummachine.h"

#include "drummachine_if.h"
#include "none_config.h"


extern double note_rates[96];

extern int bpm, beat;
extern int beat_index;
extern int tick;

int drummachine_calc_note(drummachine * rsq);
void dm_restart_beat(drummachine * rsq);
void dm_play_once(drummachine * rsq);


module_class drummachine_mif = {
	SEQUENCER,
	0,
	(char *) "drummachine",
	(char *) "Drum Machine",
	NULL,
	(void *) drummachine_new,
	(void *) drummachine_clone,
	NULL,
	NULL,
	(void *) none_config_cb,
	(void *) drummachine_if_new
};

drummachine *drummachine_new()
{
	drummachine *s;
	gint i;

	s = (drummachine *) malloc(sizeof(drummachine));
	s->module.class = &drummachine_mif;
	s->module.on = 0;
	s->module.nr_inputs = 0;
	s->module.nr_outputs = MAX_CHANNELS;
	for (i = 0; i < MAX_CHANNELS; i++) {
		s->module.outputs[i] = malloc(sizeof(channel));
		s->module.outputs[i]->parms.ch_type = CH_TYPE_SEQUENCE;
		s->module.outputs[i]->data =
		    (seq_channel *) (&(s->seq_ch[i]));
		snprintf(s->module.outputs[i]->u_label,
			 sizeof(s->module.outputs[i]->u_label), "%d", i);
		s->module.outputs[i]->module = (module *) s;
	}
	aube_module_clear_outputs((module *) s);
	snprintf(s->module.u_label, sizeof(s->module.u_label),
		 "drummachine");

	s->module.do_tick = (void *) drummachine_calc_note;

	aube_add_module((module *) s);

	s->vol = 48;
	s->pan = 16;
	s->tune = 128;
	s->dm_on = 0;
	s->dmi = 0;

	dm_clear(s);

	return s;
}

drummachine *drummachine_clone(drummachine * orsq)
{
	drummachine *s;
	gint i;

	s = (drummachine *) malloc(sizeof(drummachine));
	memcpy(s, orsq, sizeof(drummachine));

	s->module.nr_inputs = 0;
	s->module.nr_outputs = 1;
	for (i = 0; i < MAX_CHANNELS; i++) {
		s->module.outputs[i] = malloc(sizeof(channel));
		s->module.outputs[i]->parms.ch_type = CH_TYPE_SEQUENCE;
		s->module.outputs[i]->data =
		    (seq_channel *) (&(s->seq_ch[i]));
		s->module.outputs[i]->module = (module *) s;
		snprintf(s->module.outputs[i]->u_label,
			 sizeof(s->module.outputs[i]->u_label), "%d", i);
	}
	aube_module_clear_outputs((module *) s);
	snprintf(s->module.u_label, sizeof(s->module.u_label),
		 "drummachine");

	s->module.do_tick = (void *) drummachine_calc_note;

	aube_add_module((module *) s);

	return s;
}

int drummachine_calc_note(drummachine * rsq)
{
	gint i;

	rsq->bi = beat_index;

	for (i = 0; i < MAX_CHANNELS; i++) {
		rsq->seq_ch[i].vol = rsq->vol;
		rsq->seq_ch[i].accent = 0;
		rsq->seq_ch[i].pitch = 128.0;
		rsq->seq_ch[i].trigger = 0;
	}

	rsq->bi += tick;
	if (rsq->bi >= beat) {
		dm_restart_beat(rsq);
	}
	return PROC_SUCCESS;
}

void dm_restart_beat(drummachine * rsq)
{
	gint i;

	rsq->bi = 0;

	rsq->dmi++;
	rsq->dmi %= DM_LENGTH;

	for (i = 0; i < MAX_CHANNELS; i++) {
		rsq->seq_ch[i].vol = rsq->vol;
		rsq->seq_ch[i].accent = 0;
		rsq->seq_ch[i].pitch = 128.0;
		rsq->seq_ch[i].trigger =
		    (rsq->sequence[i][rsq->dmi].note > 0);
	}

}

void dm_restart(drummachine * rsq)
{
	rsq->dmi = 0;
}

void dm_shift_left(drummachine * rsq)
{
	rsq->dmi += DM_LENGTH;
	rsq->dmi--;
	rsq->dmi %= DM_LENGTH;
}

void dm_shift_right(drummachine * rsq)
{
	rsq->dmi++;
	rsq->dmi %= DM_LENGTH;
}

void dm_clear(drummachine * dm)
{
	int i, j;

	for (j = 0; j < MAX_CHANNELS; j++) {
		for (i = 0; i < DM_LENGTH; i++) {
			dm->sequence[j][i].note = 0;
		}
	}
}

void dm_chaos(drummachine * rsq)
{
	gint i, j;

	for (j = 0; j < MAX_CHANNELS; j++) {
		for (i = 0; i < DM_LENGTH; i++) {
			if ((rand() % DM_LENGTH) < DM_SILENCE) {
				rsq->sequence[j][i].note = 0;
			} else {
				rsq->sequence[j][i].note = 128;
			}
		}
	}
}

void dm_clear_track(drummachine * dm, int trackno)
{
	int i;

	for (i = 0; i < DM_LENGTH; i++) {
		dm->sequence[trackno][i].note = 0;
	}
}

void dm_shift_track(drummachine * dm, int trackno, int delta)
{
	dm_data tmp_sequence[DM_LENGTH];
	int i, j;

	memcpy(tmp_sequence, dm->sequence[trackno], sizeof(tmp_sequence));

	j = DM_LENGTH + delta;
	for (i = 0; i < DM_LENGTH; i++) {
		j %= DM_LENGTH;
		dm->sequence[trackno][j] = tmp_sequence[i];
		j++;
	}
}

void dm_play_once(drummachine * rsq)
{
	int i;

	rsq->sequence[0][0].note = 128;
	for (i = 1; i < DM_LENGTH; i++) {
		rsq->sequence[0][i].note = 0;
	}
}

void dm_play_44(drummachine * rsq)
{
	int i;

	for (i = 0; i < DM_LENGTH; i++) {
		if (!(i % 4)) {
			rsq->sequence[0][i].note = 128;
		} else {
			rsq->sequence[0][i].note = 0;
		}
	}
}

void dm_transpose_u12(drummachine * rsq)
{
	dm_transpose(rsq, 12);
}

void dm_transpose_d12(drummachine * rsq)
{
	dm_transpose(rsq, -12);
}

void dm_transpose_u1(drummachine * rsq)
{
	dm_transpose(rsq, 1);
}

void dm_transpose_d1(drummachine * rsq)
{
	dm_transpose(rsq, -1);
}

void dm_transpose(drummachine * rsq, int t)
{
	int i, nri;
	int note_rate;

	for (i = 0; i < DM_LENGTH; i++) {
		if (rsq->sequence[0][i].note > 0) {
			note_rate = rsq->sequence[0][i].note;
			for (nri = 0;
			     nri < 96
			     && ((int) ((double) 64.0 * note_rates[nri]) <=
				 note_rate); nri++);
			nri += t;
			if (nri < 0)
				nri = 0;
			if (nri >= 95)
				nri = 95;
			rsq->sequence[0][i].note =
			    (int) ((double) 64 * note_rates[nri]);
		}
	}
}
