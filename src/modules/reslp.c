// ----------------- file filterIIR00.c begin -----------------
/*
   Resonant low pass filter source code.
   By baltrax@hotmail.com (Zxform)
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "reslp.h"
#include "aube.h"


BIQUAD ProtoCoef[FILTER_SECTIONS];	/*
					   Filter prototype coefficients,
					   1 for each filter section 
					 */

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

/*
 * --------------------------------------------------------------------
 * 
 * iir_filter - Perform IIR filtering sample by sample on floats
 * 
 * Implements cascaded direct form II second order sections.
 * Requires RESLP_FILTER structure for history and coefficients.
 * The length in the filter structure specifies the number of sections.
 * The size of the history array is 2*iir->length.
 * The size of the coefficient array is 4*iir->length + 1 because
 * the first coefficient is the overall scale factor for the filter.
 * Returns one output sample for each input sample.  Allocates history
 * array if not previously allocated.
 * 
 * float iir_filter(float input,RESLP_FILTER *iir)
 * 
 *     float input        new float input sample
 *     RESLP_FILTER *iir        pointer to RESLP_FILTER structure
 * 
 * Returns float value giving the current output.
 * 
 * Allocation errors cause an error message and a call to exit.
 * --------------------------------------------------------------------
 */
float 
iir_filter(float input, RESLP_FILTER * iir)
{
  unsigned int i;
  float *hist1_ptr, *hist2_ptr, *coef_ptr;
  float output, new_hist, history1, history2;

/*
   allocate history array if different size than last call 
 */

  if (!iir->history) {
    iir->history = (float *) calloc(2 * iir->length, sizeof(float));
    if (!iir->history) {
      printf("\nUnable to allocate history array in iir_filter\n");
      exit(1);
    }
  }
  coef_ptr = iir->coef;		/*
				   coefficient pointer 
				 */

  hist1_ptr = iir->history;	/*
				   first history 
				 */
  hist2_ptr = hist1_ptr + 1;	/*
				   next history 
				 */

  /*
     1st number of coefficients array is overall input scale factor,
     * or filter gain 
   */
  output = input * (*coef_ptr++);

  for (i = 0; i < iir->length; i++) {
    history1 = *hist1_ptr;	/*
				   history values 
				 */
    history2 = *hist2_ptr;

    output = output - history1 * (*coef_ptr++);
    new_hist = output - history2 * (*coef_ptr++);	/*
							   poles 
							 */

    output = new_hist + history1 * (*coef_ptr++);
    output = output + history2 * (*coef_ptr++);		/*
							   zeros 
							 */

    *hist2_ptr++ = *hist1_ptr;
    *hist1_ptr++ = new_hist;
    hist1_ptr++;
    hist2_ptr++;
  }

  return (output);
}

RESLP_FILTER *
reslp_new()
{
  RESLP_FILTER *iir;

  iir = (RESLP_FILTER *) malloc(sizeof(RESLP_FILTER));
/*
 * Setup filter s-domain coefficients
 */
  /*
     Section 1 
   */
  ProtoCoef[0].a0 = 1.0;
  ProtoCoef[0].a1 = 0;
  ProtoCoef[0].a2 = 0;
  ProtoCoef[0].b0 = 1.0;
  ProtoCoef[0].b1 = 0.765367;
  ProtoCoef[0].b2 = 1.0;

  /*
     Section 2 
   */
  ProtoCoef[1].a0 = 1.0;
  ProtoCoef[1].a1 = 0;
  ProtoCoef[1].a2 = 0;
  ProtoCoef[1].b0 = 1.0;
  ProtoCoef[1].b1 = 1.847759;
  ProtoCoef[1].b2 = 1.0;

  iir->length = FILTER_SECTIONS;	/*
					   Number of filter sections 
					 */
  iir->history = NULL;

/*
 * Allocate array of z-domain coefficients for each filter section
 * plus filter gain variable
 */
  iir->coef = (float *) calloc(4 * iir->length + 1, sizeof(float));
  if (!iir->coef) {
    printf("Unable to allocate coef array, exiting\n");
    exit(1);
  }
  return (iir);
}

int 
reslp_calc(RESLP_FILTER * iir, double Q, double fc, double k)
{
  float *coef;
  double fs;			/*
				   Sampling frequency, cutoff frequency 
				 */
  unsigned nInd;
  double a0, a1, a2, b0, b1, b2;

  coef = iir->coef + 1;		/*
				   Skip k, or gain 
				 */

  fs = PLAYBACK_FREQ;		/*
				   Sampling frequency (Hz) 
				 */
  if (Q < 1.0)
    Q = 1.0;
  if (fc < 10)
    fc = 10.0;

/*
 * Compute z-domain coefficients for each biquad section
 * for new Cutoff Frequency and Resonance
 */
  for (nInd = 0; nInd < iir->length; nInd++) {
    a0 = ProtoCoef[nInd].a0;
    a1 = ProtoCoef[nInd].a1;
    a2 = ProtoCoef[nInd].a2;

    b0 = ProtoCoef[nInd].b0;
    b1 = ProtoCoef[nInd].b1 / Q;	/*
					   Divide by resonance or Q 
					 */
    b2 = ProtoCoef[nInd].b2;
    szxform(&a0, &a1, &a2, &b0, &b1, &b2, fc, fs, &k, coef);
    coef += 4;			/*
				   Point to next filter section 
				 */
  }

  /*
     Update overall filter gain in coef array 
   */
  iir->coef[0] = k;

/*
 * To process audio samples, call function iir_filter()
 * for each audio sample
 */
  return (1);
}
