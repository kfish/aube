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

#ifndef __ATONAL_H__
#define __ATONAL_H__

#include "aube.h"

/* random factor of silent notes */
#define AT_LENGTH 16
#define AT_SILENCE 10

typedef struct {
	int trigger;
	int note;
} at_data;

typedef struct {
	module module;
	seq_channel seq_ch;
	at_data sequence[AT_LENGTH];
	int bi;			/* beat index */
	int vol;
	int pan;
	int tune;
	int ati;
} atonal;

extern module_class atonal_mif;

atonal *atonal_new();
atonal *atonal_clone(atonal * at);
void at_restart(atonal * at);
void at_shift_left(atonal * at);
void at_shift_right(atonal * at);
void at_clear(atonal * at);
void at_chaos(atonal * at);
void at_play_once(atonal * at);
void at_play_44(atonal * at);
void at_transpose_u12(atonal * at);
void at_transpose_d12(atonal * at);
void at_transpose_u1(atonal * at);
void at_transpose_d1(atonal * at);
void at_transpose(atonal * at, int t);

#endif				/*  __ATONAL_H__  */
