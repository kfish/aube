#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <X11/Xlib.h>
#include <glib.h>
#include <gtk/gtk.h>

#include "master.h"
#include "minitracker.h"

#include "minitracker_if.h"
#include "none_config.h"


extern double note_rates[96];

extern int bpm, beat;
extern int beat_index;
extern int tick;

int minitracker_calc_note(minitracker * rsq);
void restart_beat(minitracker * rsq);
void seq_play_once(minitracker * rsq);


module_class minitracker_mif = {
  SEQUENCER,
  0,
  (char *) "minitracker",
  (char *) "Minitracker",
  NULL,
  (void *) minitracker_new,
  (void *) minitracker_clone,
  NULL,
  NULL,
  (void *) none_config_cb,
  (void *) minitracker_if_new,
  (void *) minitracker_if_close_cb
};

minitracker *
minitracker_new()
{
  minitracker *s;

  s = (minitracker *) malloc(sizeof(minitracker));
  s->module.class = &minitracker_mif;
  s->module.on = 0;
  s->module.nr_inputs = 0;
  s->module.nr_outputs = 1;
  s->module.outputs[0] = malloc(sizeof(channel));
  s->module.outputs[0]->parms.ch_type = CH_TYPE_SEQUENCE;
  s->module.outputs[0]->data = (seq_channel *) (&(s->seq_ch));
  aube_module_clear_outputs((module *) s);
  sprintf(s->module.u_label, "minitracker");
  sprintf(s->module.outputs[0]->u_label, "seq");
  s->module.outputs[0]->module = (module *) s;

  s->module.do_tick = (void *) minitracker_calc_note;

  aube_add_module((module *) s);

  s->vol = 48;
  s->pan = 16;
  s->tune = 128;
  s->portamento = 0;
  s->seq_on = 0;
  s->seqi = 0;
  s->port_sub = 0;
  s->port_val = 0;
  s->port_delta = 0;

  chaos(s);

  return s;
}

minitracker *
minitracker_clone(minitracker * orsq)
{
  minitracker *s;

  s = (minitracker *) malloc(sizeof(minitracker));
  memcpy(s, orsq, sizeof(minitracker));

  s->module.nr_inputs = 0;
  s->module.nr_outputs = 1;
  s->module.outputs[0] = malloc(sizeof(channel));
  s->module.outputs[0]->parms.ch_type = CH_TYPE_SEQUENCE;
  s->module.outputs[0]->data = (seq_channel *) (&(s->seq_ch));
  aube_module_clear_outputs((module *) s);
  sprintf(s->module.u_label, "minitracker");
  sprintf(s->module.outputs[0]->u_label, "seq");
  s->module.outputs[0]->module = (module *) s;

  s->module.do_tick = (void *) minitracker_calc_note;

  aube_add_module((module *) s);

  return s;
}

int 
minitracker_calc_note(minitracker * rsq)
{
  rsq->seq_ch.trigger = 0;	/* set to 1 when fired */
  rsq->seq_ch.pan = rsq->pan;

  rsq->bi = beat_index;

#if 0
  if (some shit about sequence being on) {
#endif

#if 0
  } else {
    rsq->seq_ch.trigger = 0;
    rsq->seq_ch.vol = 0;
    rsq->seq_ch.pan = 16;
    rsq->seq_ch.accent = 0;
  }
#endif

  rsq->bi += tick;
  if (rsq->bi >= beat) {
    restart_beat(rsq);
  }
  return PROC_SUCCESS;
}

void 
restart_beat(minitracker * rsq)
{
  int seqi;
  int note;
  int pitch;

  rsq->bi = 0;

#if 0
  if (rsq->module.on) {
#endif
    rsq->seqi++;
    rsq->seqi %= SEQ_LENGTH;

    seqi = rsq->seqi;
    while (!rsq->sequence[seqi].note) {
      seqi += SEQ_LENGTH - 1;
      seqi %= SEQ_LENGTH;
    }

    rsq->seq_ch.vol = rsq->vol;
    rsq->seq_ch.accent = rsq->sequence[seqi].accent;

    /*pitch = rsq->sequence[seqi].note;*/
    note = rsq->sequence[seqi].note;
    if (note > 96) note = 96;
    else if (note < 0) note = 0;
    pitch = note_rates[note];

    /*
       pitch = (pitch*pitch_env_val)>>11;
     */
    rsq->seq_ch.pitch = (float) ((pitch * rsq->tune) >> 7);

    if (rsq->sequence[rsq->seqi].note) {
      rsq->seq_ch.trigger = 1;
      /*
         rsq->env_i=0;
       */
      rsq->wi = 0;
    }
#if 0
  }
#endif
#if 0
  /*
     CALCULATE PORTAMENTO 
   */
  rsq->port_sub = ((beat) * 2 * rsq->portamento) >> 6;
  if (rsq->port_sub)
    rsq->port_delta = 32768 / (rsq->port_sub);
  if (rsq->portamento)
    rsq->port_val = 0;
  else
    rsq->port_val = 32768;
#endif
}

void 
seq_toggle(minitracker * rsq)
{
  rsq->seq_on = 1 - rsq->seq_on;
}

void 
seq_restart(minitracker * rsq)
{
  rsq->seqi = 0;
}

void 
shift_left(minitracker * rsq)
{
  rsq->seqi += SEQ_LENGTH;
  rsq->seqi--;
  rsq->seqi %= SEQ_LENGTH;
}

void 
shift_right(minitracker * rsq)
{
  rsq->seqi++;
  rsq->seqi %= SEQ_LENGTH;
}

void 
chaos(minitracker * rsq)
{
  int i;

  for (i = 0; i < SEQ_LENGTH; i++) {
    if ((i == 0 || rsq->sequence[i - 1].note) && ((rand() % SEQ_LENGTH) < SEQ_SILENCE)) {
      rsq->sequence[i].note = 0;
    } else {
      /*rsq->sequence[i].note = (int) ((double) 64 * note_rates[72 + (rand() % 12)]);*/
      rsq->sequence[i].note = rand() % 96;
    }
    if ((rand() % SEQ_LENGTH) < SEQ_PORTAMENTOS) {
      rsq->sequence[i].portamento_toggle = 1;
    } else {
      rsq->sequence[i].portamento_toggle = 0;
    }
    if ((rand() % SEQ_LENGTH) < SEQ_ACCENTS) {
      rsq->sequence[i].accent = 1;
    } else {
      rsq->sequence[i].accent = 0;
    }
  }
}

void 
seq_chaos8(minitracker * rsq)
{
  int i;

  for (i = 0; i < SEQ_LENGTH; i++) {
    if ((i == 0 || rsq->sequence[i - 1].note) && ((rand() % SEQ_LENGTH) < SEQ_SILENCE)) {
      rsq->sequence[i].note = 0;
    } else {
      /*      rsq->sequence[i].note = (int) ((double) 64.0 * note_rates[72 + (rand() % 12)]);*/
      rsq->sequence[i].note = rand() % 96;
    }
    if ((rand() % SEQ_LENGTH) < SEQ_PORTAMENTOS) {
      rsq->sequence[i].portamento_toggle = 1;
    } else {
      rsq->sequence[i].portamento_toggle = 0;
    }
    if ((rand() % SEQ_LENGTH) < SEQ_ACCENTS) {
      rsq->sequence[i].accent = 1;
    } else {
      rsq->sequence[i].accent = 0;
    }
    i++;
    rsq->sequence[i].note = 0;
    rsq->sequence[i].portamento_toggle = 0;
    rsq->sequence[i].accent = 0;
  }
}

void 
seq_play_once(minitracker * rsq)
{
  int i;

  /*  rsq->sequence[0].note = 64 * note_rates[72];*/
  rsq->sequence[0].note = 72;
  rsq->sequence[0].portamento_toggle = 0;
  rsq->sequence[0].accent = 0;
  for (i = 1; i < SEQ_LENGTH; i++) {
    rsq->sequence[i].note = 0;
    rsq->sequence[i].portamento_toggle = 0;
    rsq->sequence[i].accent = 0;
  }
}

void 
seq_play_44(minitracker * rsq)
{
  int i;

  for (i = 0; i < SEQ_LENGTH; i++) {
    if (!(i % 4)) {
      /*      rsq->sequence[i].note = 64 * note_rates[72];*/
      rsq->sequence[i].note = 72;
      rsq->sequence[i].portamento_toggle = 0;
      rsq->sequence[i].accent = (!i);
    } else {
      rsq->sequence[i].note = 0;
      rsq->sequence[i].portamento_toggle = 0;
      rsq->sequence[i].accent = 0;
    }
  }
}

void 
seq_transpose_u12(minitracker * rsq)
{
  seq_transpose(rsq, 12);
}

void 
seq_transpose_d12(minitracker * rsq)
{
  seq_transpose(rsq, -12);
}

void 
seq_transpose_u1(minitracker * rsq)
{
  seq_transpose(rsq, 1);
}

void 
seq_transpose_d1(minitracker * rsq)
{
  seq_transpose(rsq, -1);
}

void 
seq_transpose(minitracker * rsq, int delta)
{
  int i;
#if 0
  int nri;
  int note_rate;
#endif

  for (i = 0; i < SEQ_LENGTH; i++) {
    if (rsq->sequence[i].note > 0) {
      rsq->sequence[i].note += delta;
#if 0
      note_rate = rsq->sequence[i].note;
      for (nri = 0; nri < 96 && ((int) ((double) 64.0 * note_rates[nri]) <= note_rate); nri++);
      nri += delta;
      if (nri < 0)
	nri = 0;
      if (nri >= 95)
	nri = 95;
      rsq->sequence[i].note = (int) ((double) 64 * note_rates[nri]);
#endif
    }
  }
}
