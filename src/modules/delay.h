#ifndef __DELAY_H__
#define __DELAY_H__

#include "aube.h"

#define DELAY_BUFFER_TICKS 256
#define DELAY_BUFFER_LENGTH (DELAY_BUFFER_TICKS*tick/2)

typedef struct {
	module module;

	/* Persistent variables */
	int input_vol;
	int delay_vol;
	int pan;
	int offset;

	/* State variables */
	bit16 delay_buffer[DELAY_BUFFER_TICKS * DEFAULT_TICK];
	int dbi;
} delay;

delay *delay_new(void);
delay *delay_clone(delay * dl);

extern module_class delay_mif;

#endif				/* __DELAY_H__  */
