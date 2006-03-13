#ifndef __MINITRACKER_H__
#define __MINITRACKER_H__

#include "aube.h"
#include "reslp.h"

#define NUM_HARMONICS 32
/* random factor of silent notes */
#define SEQ_LENGTH 64
#define SEQ_SILENCE 24
#define SEQ_PORTAMENTOS 28
#define SEQ_ACCENTS 16
#if 0
/* #define SINTAB_SIZE 4096 */
/* SINTAB_SIZE = 1<<SINTAB_LOG */
#define SINTAB_LOG 18
#define SINTAB_LEN 262144
#define ENVELOPE_LEN 256
#define ENVELOPE_POINTS_PER_TICK 1
#endif


typedef struct {
  int note;
  int portamento_toggle;	/* boolean */
  int accent;
} sequence_data;

typedef struct {
  module module;
  seq_channel seq_ch;
  sequence_data sequence[SEQ_LENGTH];
  int wi;			/* waveform index */
  int bi;			/* beat index */
  int vol;
  int pan;
  int tune;
  int portamento;
  int seq_on;
  int seqi;
  int port_sub, port_val, port_delta;
} minitracker;

extern module_class minitracker_mif;

minitracker *minitracker_new();
minitracker *minitracker_clone(minitracker * orsq);
void seq_toggle(minitracker * syre_if);
void seq_restart(minitracker * syre_if);
void shift_left(minitracker * syre_if);
void shift_right(minitracker * syre_if);
void chaos(minitracker * syre_if);
void seq_chaos8(minitracker * syre_if);
void seq_play_once(minitracker * syre_if);
void seq_play_44(minitracker * syre_if);
void seq_transpose_u12(minitracker * syre_if);
void seq_transpose_d12(minitracker * syre_if);
void seq_transpose_u1(minitracker * syre_if);
void seq_transpose_d1(minitracker * syre_if);
void seq_transpose(minitracker * syre_if, int t);

#endif /* __MINITRACKER_H__  */
