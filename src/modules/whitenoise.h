#ifndef __WHITENOISE_H__
#define __WHITENOISE_H__

#include "aube.h"

typedef struct {
	module module;
	bit16 tick_buffer[2 * PLAYBACK_FREQ];	/* one second of sound */
	int vol;
	int pan;
} whitenoise;

whitenoise *whitenoise_new(void);
whitenoise *whitenoise_clone(whitenoise * own);

extern module_class whitenoise_mif;

#endif				/* __WHITENOISE_H__  */
