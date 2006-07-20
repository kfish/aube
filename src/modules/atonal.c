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
#include "atonal.h"

#include "atonal_if.h"
#include "none_config.h"


extern double note_rates[96];

extern int bpm, beat;
extern int beat_index;
extern int tick;

int atonal_calc_note(atonal *rsq);
void at_restart_beat(atonal *rsq);
void at_play_once(atonal *rsq);


module_class atonal_mif = {
  SEQUENCER,
  0,
  (char *)"atonal",
  (char *)"Atonal Loop",
  NULL,
  (void *)atonal_new,
  (void *)atonal_clone,
  NULL,
  NULL,
  (void *)none_config_cb,
  (void *)atonal_if_new,
  (void *)atonal_if_close_cb
};

atonal *atonal_new()
{
  atonal *s;

  s = (atonal *)malloc(sizeof(atonal));
  s->module.class = &atonal_mif;
  s->module.on=0;
  s->module.nr_inputs=0;
  s->module.nr_outputs=1;
  s->module.outputs[0] = new_output(CH_TYPE_SEQUENCE, &(s->module),
    (char *)"seq", (&(s->seq_ch)));

  aube_module_clear_outputs((module *)s);
  snprintf(s->module.u_label, sizeof (s->module.u_label), "atonal");

  s->module.do_tick = (void *)atonal_calc_note;

  at_clear(s);              

  s->vol=48;
  s->pan=16;
  s->tune=440;
  s->ati=0;

  aube_add_module((module *)s);

  return s;
}

atonal *atonal_clone(atonal *orsq)
{
  atonal *s;

  s = (atonal *)malloc(sizeof(atonal));
  memcpy(s, orsq, sizeof(atonal));

  s->module.nr_inputs=0;
  s->module.nr_outputs=1;
  s->module.outputs[0] = new_output(CH_TYPE_SEQUENCE, &(s->module),
    (char *)"seq", (&(s->seq_ch)));
 
  aube_module_clear_outputs((module *)s);
  snprintf(s->module.u_label, sizeof (s->module.u_label), "atonal");

  s->module.do_tick = (void *)atonal_calc_note;

  aube_add_module((module *)s);

  return s;
}

int atonal_calc_note(atonal *rsq)
{   
  rsq->bi = beat_index;

  rsq->seq_ch.vol = rsq->vol;
  rsq->seq_ch.accent = 0;
  rsq->seq_ch.pitch = rsq->sequence[rsq->ati].note;
  rsq->seq_ch.trigger = 0;

  rsq->bi += tick;
  if(rsq->bi >= beat) {
    at_restart_beat(rsq);
  }

  return PROC_SUCCESS;
}

void at_restart_beat(atonal *rsq)
{
  rsq->bi=0;

  rsq->ati++;
  rsq->ati%=AT_LENGTH;

  rsq->seq_ch.trigger = rsq->sequence[rsq->ati].trigger;

}

void at_restart(atonal *rsq)
{
  rsq->ati=0;
}

void at_shift_left(atonal *rsq)
{
  rsq->ati += AT_LENGTH;
  rsq->ati--;
  rsq->ati %= AT_LENGTH;
}

void at_shift_right(atonal *rsq)
{
  rsq->ati++;
  rsq->ati %= AT_LENGTH;
}

void at_clear(atonal *at)
{
  int i;

  for(i=0; i<AT_LENGTH; i++) {
    at->sequence[i].trigger = 0;
    at->sequence[i].note = 440;
  }
}

void at_chaos(atonal *rsq)
{
   gint i;
   
  for(i=0; i<AT_LENGTH; i++) {
    if((rand()%AT_LENGTH) < AT_SILENCE) {
      rsq->sequence[i].trigger=0;
    } else {
      rsq->sequence[i].trigger = 1;
    }
  }
}

void at_play_once(atonal *rsq)
{
  int i;

  rsq->sequence[0].note = 128;
  for(i=1; i<AT_LENGTH; i++) {
    rsq->sequence[i].note=0;
  }
}

void at_play_44(atonal *rsq)
{
  int i;

  for(i=0; i<AT_LENGTH; i++) {
    if(!(i%4)) {
      rsq->sequence[i].note = 128;
    } else {
      rsq->sequence[i].note=0;
    }
  }
}

void at_transpose_u12(atonal *rsq)
{
  at_transpose(rsq, 12);
}

void at_transpose_d12(atonal *rsq)
{
  at_transpose(rsq, -12);
}

void at_transpose_u1(atonal *rsq)
{
  at_transpose(rsq, 1);
}

void at_transpose_d1(atonal *rsq)
{
  at_transpose(rsq, -1);
}

void at_transpose(atonal *rsq, int t)
{
  int i,nri;
  int note_rate;

  for(i=0; i<AT_LENGTH; i++) {
    if(rsq->sequence[i].note>0) {
      note_rate = rsq->sequence[i].note;
      for(nri=0; nri<96 && ((int)((double)64.0 * note_rates[nri]) <= note_rate); nri++);
      nri += t;
      if(nri<0) nri=0;
      if(nri>=95) nri=95;
      rsq->sequence[i].note = (int)((double)64 * note_rates[nri]);
    }
  }
}

