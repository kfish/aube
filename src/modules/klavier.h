#ifndef __KLAVIER_H__
#define __KLAVIER_H__

#include "aube.h"

typedef struct {
	module module;
	seq_channel seq_ch;
	int last_klav_key;
	int wi;			/* waveform index */
	int bi;			/* beat index */
	int vol;
	int pan;
	int tune;
} klavier;

klavier *klavier_new();
klavier *klavier_clone(klavier * ok);

extern module_class klavier_mif;

#endif				/* __KLAVIER_H__  */
