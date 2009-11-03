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

#ifndef __WHITENOISE_H__
#define __WHITENOISE_H__

#include "aube.h"

typedef struct {
	module module;
	bit16 tick_buffer[2 * PLAYBACK_FREQ];	/* one second of sound */
	int vol;
	int pan;
} whitenoise;

whitenoise *whitenoise_new(void);
whitenoise *whitenoise_clone(whitenoise * own);

extern module_class whitenoise_mif;

#endif				/* __WHITENOISE_H__  */
