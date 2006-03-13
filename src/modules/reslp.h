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
  unsigned int length;		/*
				   size of filter 
				 */
  float *history;		/*
				   pointer to history in filter 
				 */
  float *coef;			/*
				   pointer to coefficients of filter 
				 */
} RESLP_FILTER;

#define FILTER_SECTIONS   1	/*
				   2 filter sections for 24 db/oct filter 
				 */

typedef struct {
  double a0, a1, a2;		/*
				   numerator coefficients 
				 */
  double b0, b1, b2;		/*
				   denominator coefficients 
				 */
} BIQUAD;

void szxform(
	      double *a0, double *a1, double *a2,	/*
							   numerator coefficients 
							 */
	      double *b0, double *b1, double *b2,	/*
							   denominator coefficients 
							 */
	      double fc,	/*
				   Filter cutoff frequency 
				 */
	      double fs,	/*
				   sampling rate 
				 */
	      double *k,	/*
				   overall gain factor 
				 */
	      float *coef);	/*
				   pointer to 4 iir coefficients 
				 */

float iir_filter(float input, RESLP_FILTER * iir);

RESLP_FILTER *reslp_new();
int reslp_calc(RESLP_FILTER * iir, double Q, double fc, double k);

#endif /*
          __RESLP_H__ 
        */
