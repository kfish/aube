#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <X11/Xlib.h>
#include <glib.h>
#include <gtk/gtk.h>

#include "sample_recorder.h"
#include "master.h"

#include "sample_recorder_if.h"
#include "none_config.h"

extern int bpm, beat;
extern int tick;

int sample_recorder_calc_note(sample_recorder * sr);
void sample_recorder_play_buffer(sample_recorder * sr);
void sample_recorder_stop_recording(sample_recorder * sr);

module_class sample_recorder_mif =
{
  OUTPUT,
  0,
  (char *) "s-recorder",
  (char *) "Sample Recorder",
  NULL,
  (void *) sample_recorder_new,
  (void *) sample_recorder_clone,
  NULL,
  NULL,
  (void *) none_config_cb,
  (void *) sample_recorder_if_new,
  (void *) sample_recorder_if_close_cb
};

sample_recorder *
sample_recorder_new()
{
  sample_recorder *sr;

  sr = (sample_recorder *) malloc(sizeof(sample_recorder));
  sr->module.class = &sample_recorder_mif;
  sr->module.on = 0;
  sr->module.nr_inputs = 2;
  sr->module.inputs[0] = malloc(sizeof(input));
  sr->module.inputs[0]->ch_type = CH_TYPE_AUDIO_STREAM;
  sr->module.inputs[0]->channel = NULL;
  sr->module.inputs[1] = malloc(sizeof(input));
  sr->module.inputs[1]->ch_type = CH_TYPE_SEQUENCE;
  sr->module.inputs[1]->channel = NULL;
  sr->module.nr_outputs = 1;
  sr->module.outputs[0] = malloc(sizeof(channel));
  sr->module.outputs[0]->parms.ch_type = CH_TYPE_AUDIO_STREAM;
  sr->module.outputs[0]->data = (bit16 *) (&(sr->tick_buffer));

  aube_module_clear_outputs((module *) sr);
  snprintf(sr->module.u_label, sizeof (sr->module.u_label), "s-recorder");
  snprintf(sr->module.outputs[0]->u_label, sizeof (sr->module.outputs[0]->u_label), "out");
  sr->module.outputs[0]->module = (module *) sr;

  sr->module.do_tick = (void *) sample_recorder_calc_note;

  aube_add_module((module *) sr);

  sr->length = AUBE_SR_BUFFER_TICKS * DEFAULT_TICK;
  sr->passthrough_mode = AUBE_SAMPLE_RECORDER_PASSTHROUGH_ALWAYS;
  sr->recording = 0;
  sr->record_next = 0;

  return sr;
}

sample_recorder *
sample_recorder_clone(sample_recorder * osr)
{
  sample_recorder *sr;

  sr = (sample_recorder *) malloc(sizeof(sample_recorder));
  memcpy(sr, osr, sizeof(sample_recorder));

  sr->module.nr_inputs = 2;
  sr->module.inputs[0] = malloc(sizeof(input));
  sr->module.inputs[0]->ch_type = CH_TYPE_AUDIO_STREAM;
  sr->module.inputs[0]->channel = osr->module.inputs[0]->channel;
  sr->module.inputs[1] = malloc(sizeof(input));
  sr->module.inputs[1]->ch_type = CH_TYPE_SEQUENCE;
  sr->module.inputs[1]->channel = osr->module.inputs[1]->channel;
  sr->module.nr_outputs = 1;
  sr->module.outputs[0] = malloc(sizeof(channel));
  sr->module.outputs[0]->parms.ch_type = CH_TYPE_AUDIO_STREAM;
  sr->module.outputs[0]->data = (bit16 *) (&(sr->tick_buffer));

  aube_module_clear_outputs((module *) sr);
  snprintf(sr->module.u_label, sizeof (sr->module.u_label), "s_rec");
  snprintf(sr->module.outputs[0]->u_label, sizeof (sr->module.outputs[0]->u_label), "out");
  sr->module.outputs[0]->module = (module *) sr;

  sr->module.do_tick = (void *) sample_recorder_calc_note;

  aube_add_module((module *) sr);

  return sr;
}

int 
sample_recorder_calc_note(sample_recorder * sr)
{
  unsigned int n;
  seq_channel *seq_ch;

  /*
     Check record_next 
   */
  if (sr->record_next) {
    seq_ch = (seq_channel *) sr->module.inputs[1]->channel->data;

    if (seq_ch->trigger) {
      sr->record_next = 0;
      sr->recording = 1;
      sr->rec_offset = 0;
    }
  }
  /*
     Record 
   */
  if (sr->recording) {
    n = MIN(tick, AUBE_SR_BUFFER_TICKS * DEFAULT_TICK - sr->rec_offset);
    n *= 2;
    memcpy(&sr->rec_buffer[sr->rec_offset],
	   ((bit16 *) sr->module.inputs[0]->channel->data), n);

    /*
       Stop recording if record buffer is full 
     */
    if (n < tick * 2) {
      sample_recorder_stop_recording(sr);
    } else {
      sr->rec_offset += tick;
    }
  }
  /*
     Output 
   */
  switch (sr->passthrough_mode) {
  case AUBE_SAMPLE_RECORDER_PASSTHROUGH_NEVER:
    sample_recorder_play_buffer(sr);
    break;
  case AUBE_SAMPLE_RECORDER_PASSTHROUGH_REC:
    if (sr->recording)
      memcpy(sr->tick_buffer,
	     ((bit16 *) sr->module.inputs[0]->channel->data), tick * 2);
    else
      sample_recorder_play_buffer(sr);
    break;
  case AUBE_SAMPLE_RECORDER_PASSTHROUGH_ALWAYS:
    memcpy(sr->tick_buffer,
	   ((bit16 *) sr->module.inputs[0]->channel->data), tick * 2);
    break;
  default:
    break;
  }

  return PROC_SUCCESS;
}

void 
sample_recorder_play_buffer(sample_recorder * sr)
{
  int n;
  n = MIN(tick, sr->length - sr->play_offset);
  n *= 2;
  memcpy(sr->tick_buffer, &sr->rec_buffer[sr->play_offset], n);
  if (n < tick * 2) {
    memcpy(&sr->tick_buffer[n / 2],
	   &sr->rec_buffer[sr->play_offset], tick * 2 - n);
  }
  sr->play_offset += tick;
  sr->play_offset %= sr->length;
}

void 
sample_recorder_stop_recording(sample_recorder * sr)
{
  sr->recording = 0;
  sr->length = sr->rec_offset;
  sr->rec_offset = 0;
  if (sr->passthrough_mode == AUBE_SAMPLE_RECORDER_PASSTHROUGH_REC) {
    sr->play_offset = 0;
  }
}

void 
sample_recorder_add_sample(sample_recorder * sr, char *samplename)
{
  channel *ch;

  ch = malloc(sizeof(channel));
  ch->parms.d.audio_sample_data.s_length = sr->length;
  ch->module = NULL;
  ch->data = malloc(sr->length * 2);
  memcpy(ch->data, sr->rec_buffer, sr->length * 2);
  strncpy(ch->u_label, samplename, LABEL_LEN);

  aube_add_sample(ch);
}
