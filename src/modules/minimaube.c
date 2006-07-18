#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <X11/Xlib.h>
#include <glib.h>
#include <gtk/gtk.h>

#include "minimaube.h"
#include "master.h"

#include "minimaube_if.h"
#include "none_config.h"

extern int bpm, beat;
extern int tick;

int minimaube_calc_note(minimaube * s);

module_class minimaube_mif = {
  GENERATOR,
  0,
  (char *) "s-player",
  (char *) "Sample Player",
  NULL,
  (void *) minimaube_new,
  (void *) minimaube_clone,
  NULL,
  NULL,
  (void *) none_config_cb,
  (void *) minimaube_if_new,
  (void *) minimaube_if_close_cb
};

minimaube *
minimaube_new()
{
  minimaube *s;
  int i;

  s = (minimaube *) malloc(sizeof(minimaube));
  s->module.class = &minimaube_mif;

  s->module.nr_inputs = MAX_CHANNELS;
  s->module.nr_outputs = MAX_CHANNELS / 2;
  s->module.on = 0;
  for (i = 0; i < MAX_CHANNELS / 2; i++) {
    s->module.inputs[i * 2] = malloc(sizeof(input));
    s->module.inputs[i * 2]->ch_type = CH_TYPE_AUDIO_SAMPLE;
    s->module.inputs[i * 2]->channel = NULL;
    s->module.inputs[i * 2 + 1] = malloc(sizeof(input));
    s->module.inputs[i * 2 + 1]->ch_type = CH_TYPE_SEQUENCE;
    s->module.inputs[i * 2 + 1]->channel = NULL;
    s->module.outputs[i] = malloc(sizeof(channel));
    s->module.outputs[i]->parms.ch_type = CH_TYPE_AUDIO_STREAM;
    s->module.outputs[i]->data = (bit16 *) (&(s->tick_buffer[i]));
    snprintf(s->module.outputs[i]->u_label, sizeof (s->module.outputs[i]->u_label), "%d", i);
    s->module.outputs[i]->module = (module *) s;
  }
  aube_module_clear_outputs((module *) s);
  snprintf(s->module.u_label, sizeof (s->module.u_label), "s-player");

  s->module.do_tick = (void *) minimaube_calc_note;

  aube_add_module((module *) s);

  s->nr_active_channels = 1;
  for (i = 0; i < MAX_CHANNELS; i++) {
    s->mm[i].offset = 0;
    s->mm[i].input_vol = 64;
    s->mm[i].minimaube_vol = 16;
    s->mm[i].pan = 16;
    s->mm[i].si = 0;
  }

  return s;
}

minimaube *
minimaube_clone(minimaube * os)
{
  minimaube *s;

  s = (minimaube *) malloc(sizeof(minimaube));
  memcpy(s, os, sizeof(minimaube));

  s->module.nr_inputs = 2;
  s->module.inputs[0] = malloc(sizeof(input));
  s->module.inputs[0]->ch_type = CH_TYPE_AUDIO_SAMPLE;
  s->module.inputs[0]->channel = os->module.inputs[0]->channel;
  s->module.inputs[1] = malloc(sizeof(input));
  s->module.inputs[1]->ch_type = CH_TYPE_SEQUENCE;
  s->module.inputs[1]->channel = os->module.inputs[0]->channel;
  s->module.nr_outputs = 1;
  s->module.outputs[0] = malloc(sizeof(channel));
  s->module.outputs[0]->parms.ch_type = CH_TYPE_AUDIO_STREAM;
  s->module.outputs[0]->data = (bit16 *) (&(s->tick_buffer));

  aube_module_clear_outputs((module *) s);
  snprintf(s->module.u_label, sizeof (s->module.u_label), "minimaube");
  snprintf(s->module.outputs[0]->u_label, sizeof (s->module.outputs[0]->u_label), "out");
  s->module.outputs[0]->module = (module *) s;

  s->module.do_tick = (void *) minimaube_calc_note;

  aube_add_module((module *) s);

  return s;
}

int 
minimaube_calc_note(minimaube * s)
{
  unsigned int i;
  int j;
  int lpan, rpan;
  seq_channel *seq_ch;
  bit16 *d;
  double pitch;
  double p;
  int sii;
  input * sample_input;
  input * seq_input;
  int sample_length;

  for (j = 0; j < s->nr_active_channels; j++) {
    sample_input = s->module.inputs[j * 2];
    seq_input = s->module.inputs[j * 2 + 1];
    if ((sample_input->channel != NULL) &&
	(seq_input->channel != NULL)) {

      seq_ch = (seq_channel *) seq_input->channel->data;

      if (seq_ch->pan <= 16) {
	lpan = seq_ch->pan;
	rpan = 16;
      } else {
	lpan = 16;
	rpan = 31 - seq_ch->pan;
      }

      if (seq_ch->trigger) {
	s->mm[j].si = 0.0;
      }
      if (s->mm[j].si >= 0.0) {
	pitch = (2.0 * seq_ch->pitch / 130.81);	 /* C */
	d = (bit16 *) sample_input->channel->data;
	if (d != NULL) {
	  for (i = 0; i < tick; i += 2) {
	    sii = (int) s->mm[j].si;
	    sii -= sii % 2;

            sample_length =
              sample_input->channel->parms.d.audio_sample_data.s_length;

	    if (sii+3 > sample_length) {
	      s->mm[j].si = -1.0;
	      memset(&s->tick_buffer[j][i], 0, (tick - i) * 2);
	      i = tick;
	    } else {
	      p = s->mm[j].si - (double) sii;
	      s->tick_buffer[j][i] =
                (bit16) (p * d[sii] + (1 - p) * d[sii + 2]);
	      s->tick_buffer[j][i + 1] =
                (bit16) (p * d[sii + 1] + (1 - p) * d[sii + 3]);

	      s->mm[j].si += pitch;
            }
	  }
	} else {
	  perror("d==NULL");
	}
      } else {
	memset(s->tick_buffer[j], 0, tick * 2);
      }
    } else {
      memset(s->tick_buffer[j], 0, tick * 2);
    }
  }

  return PROC_SUCCESS;
}


int 
minimaube_add_input(minimaube * mm)
{
  if (mm->nr_active_channels < MAX_CHANNELS / 2) {
    mm->nr_active_channels++;
    return (mm->nr_active_channels - 1);
  } else {
    return 0;
  }
}

/*
   returns index of removed input 
 */
int 
minimaube_remove_input(minimaube * mm)
{
  if (mm->nr_active_channels > 1) {
    mm->nr_active_channels--;
    return mm->nr_active_channels;
  } else {
    return 0;
  }
}
