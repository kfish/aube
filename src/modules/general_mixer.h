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

#ifndef __GENERAL_MIXER_H__
#define __GENERAL_MIXER_H__

#include "aube.h"

#define MAX_INPUTS MAX_CHANNELS

typedef struct {
	module module;
	bit16 tick_buffer[2 * PLAYBACK_FREQ];	/* one second of sound */
	int nr_active_channels;
	int vol[MAX_INPUTS];
	int mute[MAX_INPUTS];
	int master_vol;
	int master_mute;
} general_mixer;

general_mixer *general_mixer_new(void);
general_mixer *general_mixer_clone(general_mixer * oxf);
int general_mixer_add_input(general_mixer * gm);
int general_mixer_remove_input(general_mixer * gm);

extern module_class general_mixer_mif;

#endif				/* __GENERAL_MIXER_H__  */
