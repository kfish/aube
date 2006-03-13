#ifndef __DRUMMACHINE_H__
#define __DRUMMACHINE_H__

#include "aube.h"

/* random factor of silent notes */
#define DM_LENGTH 32
#define DM_SILENCE 24

typedef struct {
  int note;
} dm_data;

typedef struct {
  module module;
  seq_channel seq_ch[MAX_CHANNELS];
  dm_data sequence[MAX_CHANNELS][DM_LENGTH];
  int bi;			/* beat index */
  int vol;
  int pan;
  int tune;
  int dm_on;
  int dmi;
} drummachine;

extern module_class drummachine_mif;

drummachine *drummachine_new();
drummachine *drummachine_clone(drummachine * dm);
void dm_restart(drummachine * dm);
void dm_shift_left(drummachine * dm);
void dm_shift_right(drummachine * dm);
void dm_clear(drummachine * dm);
void dm_chaos(drummachine * dm);
void dm_clear_track (drummachine * dm, int trackno);
void dm_shift_track (drummachine * dm, int trackno, int delta);
void dm_play_once(drummachine * dm);
void dm_play_44(drummachine * dm);
void dm_transpose_u12(drummachine * dm);
void dm_transpose_d12(drummachine * dm);
void dm_transpose_u1(drummachine * dm);
void dm_transpose_d1(drummachine * dm);
void dm_transpose(drummachine * dm, int t);

#endif /* __DRUMMACHINE_H__  */
