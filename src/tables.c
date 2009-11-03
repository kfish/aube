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


char *note_names[97] = {
	"---",
	"C-0", "C#0", "D-0", "D#0", "E-0", "F-0", "F#0", "G-0", "G#0",
	    "A-0", "Bb0", "B-0",
	"C-1", "C#1", "D-1", "D#1", "E-1", "F-1", "F#1", "G-1", "G#1",
	    "A-1", "Bb1", "B-1",
	"C-2", "C#2", "D-2", "D#2", "E-2", "F-2", "F#2", "G-2", "G#2",
	    "A-2", "Bb2", "B-2",
	"C-3", "C#3", "D-3", "D#3", "E-3", "F-3", "F#3", "G-3", "G#3",
	    "A-3", "Bb3", "B-3",
	"C-4", "C#4", "D-4", "D#4", "E-4", "F-4", "F#4", "G-4", "G#4",
	    "A-4", "Bb4", "B-4",
	"C-5", "C#5", "D-5", "D#5", "E-5", "F-5", "F#5", "G-5", "G#5",
	    "A-5", "Bb5", "B-5",
	"C-6", "C#6", "D-6", "D#6", "E-6", "F-6", "F#6", "G-6", "G#6",
	    "A-6", "Bb6", "B-6",
	"C-7", "C#7", "D-7", "D#7", "E-7", "F-7", "F#7", "G-7", "G#7",
	    "A-7", "Bb7", "B-7",
};

double note_rates[96] = {

	0.031250, 0.033108, 0.035077, 0.037163, 0.039373, 0.041714,
	0.044194, 0.046822, 0.049606, 0.052556, 0.055681, 0.058992,

	0.062500, 0.066216, 0.070154, 0.074325, 0.078745, 0.083427,
	0.088388, 0.093644, 0.099213, 0.105112, 0.111362, 0.117984,

	0.125000, 0.132433, 0.140308, 0.148651, 0.157490, 0.166855,
	0.176777, 0.187288, 0.198425, 0.210224, 0.222725, 0.235969,

	0.250000, 0.264866, 0.280616, 0.297302, 0.314980, 0.333710,
	0.353553, 0.374577, 0.396850, 0.420448, 0.445449, 0.471937,

	0.500000, 0.529732, 0.561231, 0.594604, 0.629961, 0.667420,
	0.707107, 0.749154, 0.793701, 0.840896, 0.890899, 0.943874,

	1.000000, 1.059463, 1.122462, 1.189207, 1.259921, 1.334840,
	1.414214, 1.498307, 1.587401, 1.681793, 1.781797, 1.887749,

	2.000000, 2.118926, 2.244924, 2.378414, 2.519842, 2.669680,
	2.828427, 2.996614, 3.174802, 3.363586, 3.563595, 3.775497,

	4.000000, 4.237852, 4.489848, 4.756828, 5.039684, 5.339359,
	5.656854, 5.993228, 6.349604, 6.727171, 7.127190, 7.550995,

};
