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

#ifndef __MINIMAUBE_H__
#define __MINIMAUBE_H__

#include "aube.h"

typedef struct {
	int offset;
	int input_vol;
	int minimaube_vol;
	int pan;
	float si;
} mm_data;

typedef struct {
	module module;
	bit16 tick_buffer[MAX_CHANNELS][DEFAULT_TICK];
	mm_data mm[MAX_CHANNELS];
	int nr_active_channels;
} minimaube;

minimaube *minimaube_new(void);
minimaube *minimaube_clone(minimaube * s);
int minimaube_add_input(minimaube * mm);
int minimaube_remove_input(minimaube * mm);

extern module_class minimaube_mif;

#endif				/* __MINIMAUBE_H__  */
