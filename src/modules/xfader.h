#ifndef __XFADER_H__
#define __XFADER_H__

#include "aube.h"

typedef struct {
	module module;
	bit16 tick_buffer[2 * PLAYBACK_FREQ];	/* one second of sound */
	int vol0, vol1;
	int fader;
} xfader;

xfader *xfader_new(void);
xfader *xfader_clone(xfader * oxf);

extern module_class xfader_mif;

#endif				/* __XFADER_H__  */
