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

#ifndef __PANNER4_H__
#define __PANNER4_H__

#include "aube.h"

typedef struct {
	module module;
	bit16 fwd_buffer[DEFAULT_TICK];
	bit16 rear_buffer[DEFAULT_TICK];
	int vol;
	int pan;
	int fade;
} panner4;

panner4 *panner4_new(void);
panner4 *panner4_clone(panner4 * opn);

extern module_class panner4_mif;

#endif				/* __PANNER4_H__  */
