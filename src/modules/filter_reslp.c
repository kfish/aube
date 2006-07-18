#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <X11/Xlib.h>
#include <glib.h>
#include <gtk/gtk.h>

#include "filter_reslp.h"
#include "master.h"
#include "waves.h"

#include "filter_reslp_if.h"
#include "none_config.h"

extern double note_rates[96];

extern int bpm, beat;
extern int tick;

int filter_reslp_calc_note(filter_reslp * filter_reslp);
void restart_beat(filter_reslp * filter_reslp);
void calc_filter(filter_reslp * filter_reslp);

module_class filter_reslp_mif = {
  FILTER,
  0,
  (char *) "reslp",
  (char *) "Resonant Low Pass",
  NULL,
  (void *) filter_reslp_new,
  (void *) filter_reslp_clone,
  NULL,
  NULL,
  (void *) none_config_cb,
  (void *) filter_reslp_if_new,
  (void *) filter_reslp_if_close_cb
};

void 
filter_reslp_init()
{
}

filter_reslp *
filter_reslp_new()
{
  filter_reslp *s;
  int i;

  s = (filter_reslp *) malloc(sizeof(filter_reslp));
  s->module.class = &filter_reslp_mif;
  s->module.on = 0;
  s->module.nr_inputs = 2;
  s->module.inputs[0] = malloc(sizeof(input));
  s->module.inputs[0]->ch_type = CH_TYPE_AUDIO_STREAM;
  s->module.inputs[0]->channel = NULL;
  s->module.inputs[1] = malloc(sizeof(input));
  s->module.inputs[1]->ch_type = CH_TYPE_SEQUENCE;
  s->module.inputs[1]->channel = NULL;
  s->module.nr_outputs = 1;
  s->module.outputs[0] = malloc(sizeof(channel));
  s->module.outputs[0]->parms.ch_type = CH_TYPE_AUDIO_STREAM;
  s->module.outputs[0]->data = (bit16 *) (&(s->tick_buffer));
  aube_module_clear_outputs((module *) s);
  snprintf(s->module.u_label, sizeof (s->module.u_label), "reslp");
  snprintf(s->module.outputs[0]->u_label, sizeof (s->module.outputs[0]->u_label), "out");
  s->module.outputs[0]->module = (module *) s;

  s->module.do_tick = (void *) filter_reslp_calc_note;

  aube_add_module((module *) s);

  s->wi = 0;
  s->bi = 0;
  s->vol = 48;
  s->pan = 16;
  for (i = 0; i < ENVELOPE_LEN; i++) {
    s->resonance_envelope[i] = 50;
  }
  for (i = 0; i < ENVELOPE_LEN; i++) {
    s->cutoff_envelope[i] = 50;
  }
  s->resonance_base = 1;
  s->resonance = 1;
  s->cutoff_base = 20000;
  s->cutoff = 20000;
  s->gain = 100;
  s->env_i = 0;
  s->use_trigger = 0;

  s->iir = reslp_new();
  calc_filter(s);

  return s;
}

filter_reslp *
filter_reslp_clone(filter_reslp * ofr)
{
  filter_reslp *s;

  s = (filter_reslp *) malloc(sizeof(filter_reslp));
  memcpy(s, ofr, sizeof(filter_reslp));
  s->module.nr_inputs = 2;
  s->module.inputs[0] = malloc(sizeof(input));
  s->module.inputs[0]->ch_type = CH_TYPE_AUDIO_STREAM;
  s->module.inputs[0]->channel = ofr->module.inputs[0]->channel;
  s->module.inputs[1] = malloc(sizeof(input));
  s->module.inputs[1]->ch_type = CH_TYPE_SEQUENCE;
  s->module.inputs[1]->channel = ofr->module.inputs[1]->channel;
  s->module.nr_outputs = 1;
  s->module.outputs[0] = malloc(sizeof(channel));
  s->module.outputs[0]->parms.ch_type = CH_TYPE_AUDIO_STREAM;
  s->module.outputs[0]->data = (bit16 *) (&(s->tick_buffer));
  aube_module_clear_outputs((module *) s);
  snprintf(s->module.u_label, sizeof (s->module.u_label), "reslp");
  snprintf(s->module.outputs[0]->u_label, sizeof (s->module.outputs[0]->u_label), "out");
  s->module.outputs[0]->module = (module *) s;

  s->module.do_tick = (void *) filter_reslp_calc_note;

  aube_add_module((module *) s);

  s->iir = reslp_new();
  calc_filter(s);

  return s;
}

int 
filter_reslp_calc_note(filter_reslp * filter_reslp)
{
  unsigned int i, j, k;
  int spq;			/*
				   samples per tick 
				 */
  /*
     int lpan, rpan;
   */
  seq_channel *seq_ch;

  i = 0;

  if (filter_reslp->use_trigger == 1 && filter_reslp->module.inputs[1]->channel != NULL) {

    seq_ch = (seq_channel *) filter_reslp->module.inputs[1]->channel->data;

#if 0
    if (filter_reslp->pan <= 16) {
      lpan = filter_reslp->pan;
      rpan = 16;
    } else {
      lpan = 16;
      rpan = 31 - filter_reslp->pan;
    }
#endif

    spq = tick / ENVELOPE_POINTS_PER_TICK;

    if (seq_ch->trigger) {
      filter_reslp->env_i = 0;
    }
    for (j = 0; j < ENVELOPE_POINTS_PER_TICK; j++) {
      if (filter_reslp->env_i >= 0) {


	filter_reslp->resonance = (double) filter_reslp->resonance_base * ((double) filter_reslp->resonance_envelope[filter_reslp->env_i]) / 50.0;
	filter_reslp->cutoff = (double) filter_reslp->cutoff_base * ((double) filter_reslp->cutoff_envelope[filter_reslp->env_i]) / 50.0;
	calc_filter(filter_reslp);

	for (k = 0; k < spq; k += 2) {
	  filter_reslp->tick_buffer[i] =
	    (bit16) iir_filter((float) (((bit16 *) filter_reslp->module.inputs[0]->channel->data)[i]), filter_reslp->iir);

	  filter_reslp->tick_buffer[i + 1] = filter_reslp->tick_buffer[i];
#if 0
	  filter_reslp->tick_buffer[i + 1] =
	    (bit16) ((filter_reslp->tick_buffer[i] * rpan) >> 5);
	  filter_reslp->tick_buffer[i] =
	    (bit16) ((filter_reslp->tick_buffer[i] * lpan) >> 5);
#endif

	  i += 2;

	}
      } else {
	for (k = 0; k < spq; k++) {
	  filter_reslp->tick_buffer[i] = (bit16) 0;
	  i++;
	}
      }

      if (filter_reslp->env_i > -1) {
	filter_reslp->env_i++;
	if (filter_reslp->env_i > ENVELOPE_LEN)
	  filter_reslp->env_i = -1;
      }
    }				/*
				   j 
				 */
  } else if (filter_reslp->use_trigger == 0) {
    filter_reslp->env_i = -1;
    filter_reslp->resonance = (double) filter_reslp->resonance_base;
    filter_reslp->cutoff = (double) filter_reslp->cutoff_base;
    calc_filter(filter_reslp);

    for (i = 0; i < tick; i++) {
      filter_reslp->tick_buffer[i] =
	(bit16) iir_filter((float) (((bit16 *) filter_reslp->module.inputs[0]->channel->data)[i]), filter_reslp->iir);

      filter_reslp->tick_buffer[i + 1] = filter_reslp->tick_buffer[i];
    }
  } else {
    for (k = 0; k < tick; k++) {
      filter_reslp->tick_buffer[k] = (bit16) 0;
    }
  }


  return PROC_SUCCESS;
}

void 
calc_filter(filter_reslp * filter_reslp)
{
  reslp_calc(filter_reslp->iir, (double) filter_reslp->resonance, (double) filter_reslp->cutoff, (double) filter_reslp->gain / 100.0);
}
