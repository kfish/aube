#ifndef __MINIMAUBE_H__
#define __MINIMAUBE_H__

#include "aube.h"

typedef struct {
	int offset;
	int input_vol;
	int minimaube_vol;
	int pan;
	float si;
} mm_data;

typedef struct {
	module module;
	bit16 tick_buffer[MAX_CHANNELS][DEFAULT_TICK];
	mm_data mm[MAX_CHANNELS];
	int nr_active_channels;
} minimaube;

minimaube *minimaube_new(void);
minimaube *minimaube_clone(minimaube * s);
int minimaube_add_input(minimaube * mm);
int minimaube_remove_input(minimaube * mm);

extern module_class minimaube_mif;

#endif				/* __MINIMAUBE_H__  */
