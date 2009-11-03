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

#ifndef __DELAY_H__
#define __DELAY_H__

#include "aube.h"

#define DELAY_BUFFER_TICKS 256
#define DELAY_BUFFER_LENGTH (DELAY_BUFFER_TICKS*tick/2)

typedef struct {
	module module;

	/* Persistent variables */
	int input_vol;
	int delay_vol;
	int pan;
	int offset;

	/* State variables */
	bit16 delay_buffer[DELAY_BUFFER_TICKS * DEFAULT_TICK];
	int dbi;
} delay;

delay *delay_new(void);
delay *delay_clone(delay * dl);

extern module_class delay_mif;

#endif				/* __DELAY_H__  */
