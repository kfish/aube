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

#include <stdio.h>
#include <stdlib.h>		/*
				   for random() 
				 */
#include <math.h>

#include "syre.h"

bit16 sawtooth[SINTAB_LEN];
bit16 squarewave[SINTAB_LEN];
bit16 sinwave[SINTAB_LEN];
bit16 wnwave[SINTAB_LEN];

void generate_squarewave(void)
{
	int i;

	for (i = 0; i < SINTAB_LEN / 2; i++) {
		squarewave[i] = 32768 / 2;
	}
	for (; i < SINTAB_LEN; i++) {
		squarewave[i] = -(32768 / 2);
	}
}

void generate_sawtooth(void)
{
	int i;

	for (i = 0; i < SINTAB_LEN; i++) {
		sawtooth[i] = (i * 32768) / SINTAB_LEN - (32768 / 2);
	}
}

void generate_sinwave(void)
{
	int i;

	for (i = 0; i < (SINTAB_LEN); i++) {
		sinwave[i] =
		    (bit16) (32768.0 * sin(2.0 * M_PI * i / (SINTAB_LEN)));
	}
}

void generate_wnwave(void)
{
	int i;

	for (i = 0; i < SINTAB_LEN; i++) {
		wnwave[i] = (bit16) ((random() % (1 << 16)) - (1 << 15));
	}
}
