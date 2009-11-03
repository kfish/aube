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

#ifndef __RESLP_H__
#define __RESLP_H__
/*
   Resonant low pass filter source code.
   By baltrax@hotmail.com (Zxform)
 */

/*
   FILTER INFORMATION STRUCTURE FOR FILTER ROUTINES 
 */

typedef struct {
	unsigned int length;	/*
				   size of filter 
				 */
	float *history;		/*
				   pointer to history in filter 
				 */
	float *coef;		/*
				   pointer to coefficients of filter 
				 */
} RESLP_FILTER;

#define FILTER_SECTIONS   1	/*
				   2 filter sections for 24 db/oct filter 
				 */

typedef struct {
	double a0, a1, a2;	/*
				   numerator coefficients 
				 */
	double b0, b1, b2;	/*
				   denominator coefficients 
				 */
} BIQUAD;

void szxform(double *a0, double *a1, double *a2,	/*
							   numerator coefficients 
							 */
	     double *b0, double *b1, double *b2,	/*
							   denominator coefficients 
							 */
	     double fc,		/*
				   Filter cutoff frequency 
				 */
	     double fs,		/*
				   sampling rate 
				 */
	     double *k,		/*
				   overall gain factor 
				 */
	     float *coef);	/*
				   pointer to 4 iir coefficients 
				 */

float iir_filter(float input, RESLP_FILTER * iir);

RESLP_FILTER *reslp_new();
int reslp_calc(RESLP_FILTER * iir, double Q, double fc, double k);

#endif				/*
				   __RESLP_H__ 
				 */
