#ifndef __SYRE_H__
#define __SYRE_H__

#include "aube.h"
#include "reslp.h"

#define NUM_HARMONICS 32
/* #define SINTAB_SIZE 4096 */
/*
   SINTAB_SIZE = 1<<SINTAB_LOG 
 */
#define SINTAB_LOG 18
#define SINTAB_LEN 262144
#define ENVELOPE_LEN 256
#define ENVELOPE_POINTS_PER_TICK 1
#define TICKS_PER_ENVELOPE_POINT 4


typedef struct {
	int si, r;
	int vol;
} harmonic;

typedef struct {
	module module;
	bit16 tick_buffer[2 * PLAYBACK_FREQ];	/* one second of sound */
	harmonic overtones[NUM_HARMONICS];
	bit16 *current_wave;
	bit32 waveform[SINTAB_LEN];
	int waveform_length;
	float wi;		/* waveform index */
	int bi;			/* beat index */
	int tune;
#if 0
	int vol;
	int pan;
#endif
	float amp_envelope[ENVELOPE_LEN];
	float pitch_envelope[ENVELOPE_LEN];
	float amp_envelope_accented[ENVELOPE_LEN];
	float pitch_envelope_accented[ENVELOPE_LEN];
	float freq_envelope[ENVELOPE_LEN];
	int env_i;
	int cleanbuffer;	/* boolean, true if buffer all zero */
	int harmonics_changed;	/* boolean */
} syre_if_data;

void syre_if_data_init();


syre_if_data *syre_new(void);
syre_if_data *syre_clone(syre_if_data * os);
void chaos_harmonics(syre_if_data * syre_if);
void clear_harmonics(syre_if_data * syre_if);
void clear_odd_harmonics(syre_if_data * syre_if);

extern module_class syre_mif;

#endif				/* __SYRE_H__  */
