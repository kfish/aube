#ifndef __GENERAL_MIXER_H__
#define __GENERAL_MIXER_H__

#include "aube.h"

#define MAX_INPUTS MAX_CHANNELS

typedef struct {
	module module;
	bit16 tick_buffer[2 * PLAYBACK_FREQ];	/* one second of sound */
	int nr_active_channels;
	int vol[MAX_INPUTS];
	int mute[MAX_INPUTS];
	int master_vol;
	int master_mute;
} general_mixer;

general_mixer *general_mixer_new(void);
general_mixer *general_mixer_clone(general_mixer * oxf);
int general_mixer_add_input(general_mixer * gm);
int general_mixer_remove_input(general_mixer * gm);

extern module_class general_mixer_mif;

#endif				/* __GENERAL_MIXER_H__  */
