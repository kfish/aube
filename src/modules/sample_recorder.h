#ifndef __SAMPLE_RECORDER_H__
#define __SAMPLE_RECORDER_H__

#include "aube.h"

#define AUBE_SAMPLE_RECORDER_PASSTHROUGH_NEVER   0
#define AUBE_SAMPLE_RECORDER_PASSTHROUGH_REC     1
#define AUBE_SAMPLE_RECORDER_PASSTHROUGH_ALWAYS  2

#define AUBE_SR_BUFFER_TICKS 102400

typedef struct {
  module module;
  bit16 tick_buffer[DEFAULT_TICK];
  bit16 rec_buffer[AUBE_SR_BUFFER_TICKS * DEFAULT_TICK];
  int length;			/* length of rec_buffer */
  int play_offset;
  int rec_offset;
  int passthrough_mode;
  int recording;
  int record_next;
} sample_recorder;

sample_recorder *sample_recorder_new(void);
sample_recorder *sample_recorder_clone(sample_recorder * osr);
void sample_recorder_stop_recording(sample_recorder * sr);
void sample_recorder_add_sample(sample_recorder * sr, char *samplename);

extern module_class sample_recorder_mif;

#endif /* __SAMPLE_RECORDER_H__  */
