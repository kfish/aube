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

#ifndef __FILTERRESLP_H__
#define __FILTERRESLP_H__

#include "aube.h"
#include "reslp.h"

#define ENVELOPE_LEN 256
#define ENVELOPE_POINTS_PER_TICK 1

typedef struct {
	module module;
	bit16 tick_buffer[2 * PLAYBACK_FREQ];	/* one second of sound */
	int wi;			/* waveform index */
	int bi;			/* beat index */
	int vol;
	int pan;
	float cutoff_envelope[ENVELOPE_LEN];
	float resonance_envelope[ENVELOPE_LEN];
	RESLP_FILTER *iir;
	int resonance_base, resonance;
	int cutoff_base, cutoff;
	int gain;
	int use_trigger;
	int env_i;
} filter_reslp;

void filter_reslp_init();
filter_reslp *filter_reslp_new(void);
filter_reslp *filter_reslp_clone(filter_reslp * ofr);

extern module_class filter_reslp_mif;

#endif				/* __FILTERRESLP_H__  */
