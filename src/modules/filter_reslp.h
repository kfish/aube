#ifndef __FILTERRESLP_H__
#define __FILTERRESLP_H__

#include "aube.h"
#include "reslp.h"

#define ENVELOPE_LEN 256
#define ENVELOPE_POINTS_PER_TICK 1

typedef struct {
	module module;
	bit16 tick_buffer[2 * PLAYBACK_FREQ];	/* one second of sound */
	int wi;			/* waveform index */
	int bi;			/* beat index */
	int vol;
	int pan;
	float cutoff_envelope[ENVELOPE_LEN];
	float resonance_envelope[ENVELOPE_LEN];
	RESLP_FILTER *iir;
	int resonance_base, resonance;
	int cutoff_base, cutoff;
	int gain;
	int use_trigger;
	int env_i;
} filter_reslp;

void filter_reslp_init();
filter_reslp *filter_reslp_new(void);
filter_reslp *filter_reslp_clone(filter_reslp * ofr);

extern module_class filter_reslp_mif;

#endif				/* __FILTERRESLP_H__  */
