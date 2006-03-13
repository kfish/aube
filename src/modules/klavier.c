#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <X11/Xlib.h>
#include <glib.h>
#include <gtk/gtk.h>

#include "master.h"
#include "klavier.h"

#include "klavier_if.h"
#include "none_config.h"

extern double note_rates[96];

extern int bpm, beat;
extern int tick;

int klavier_calc_note(klavier * rsq);
void restart_beat(klavier * rsq);

module_class klavier_mif =
{
  CONTROLLER,
  0,
  (char *) "klavier",
  (char *) "Klavier",
  NULL,
  (void *) klavier_new,
  (void *) klavier_clone,
  NULL,
  NULL,
  (void *) none_config_cb,
  (void *) klavier_if_new,
  (void *) klavier_if_close_cb
};

klavier *
klavier_new()
{
  klavier *s;

  s = (klavier *) malloc(sizeof(klavier));
  s->module.class = &klavier_mif;
  s->module.on = 0;
  s->module.nr_inputs = 0;
  s->module.nr_outputs = 1;
  s->module.outputs[0] = malloc(sizeof(channel));
  s->module.outputs[0]->parms.ch_type = CH_TYPE_SEQUENCE;
  s->module.outputs[0]->data = (seq_channel *) (&(s->seq_ch));
  aube_module_clear_outputs((module *) s);
  sprintf(s->module.u_label, "klavier");
  sprintf(s->module.outputs[0]->u_label, "seq");
  s->module.outputs[0]->module = (module *) s;

  s->module.do_tick = (void *) klavier_calc_note;

  aube_add_module((module *) s);

  s->last_klav_key = -1;
  s->vol = 48;
  s->pan = 16;
  s->tune = 128;

  return s;
}

klavier *
klavier_clone(klavier * ok)
{
  klavier *s;

  s = (klavier *) malloc(sizeof(klavier));
  memcpy(s, ok, sizeof(klavier));
  s->module.nr_inputs = 0;
  s->module.nr_outputs = 1;
  s->module.outputs[0] = malloc(sizeof(channel));
  s->module.outputs[0]->parms.ch_type = CH_TYPE_SEQUENCE;
  s->module.outputs[0]->data = (seq_channel *) (&(s->seq_ch));
  aube_module_clear_outputs((module *) s);
  sprintf(s->module.u_label, "klavier");
  sprintf(s->module.outputs[0]->u_label, "seq");
  s->module.outputs[0]->module = (module *) s;

  s->module.do_tick = (void *) klavier_calc_note;

  aube_add_module((module *) s);

  return s;
}

int 
klavier_calc_note(klavier * rsq)
{
  rsq->seq_ch.trigger = 0;	/*
				   set to 1 when fired 
				 */
  rsq->seq_ch.pan = rsq->pan;

  rsq->seq_ch.vol = rsq->vol;;
  rsq->seq_ch.accent = 0;

  if (rsq->module.on && rsq->last_klav_key > 0) {
    rsq->seq_ch.trigger = 1;
    rsq->seq_ch.vol = rsq->vol;;
    rsq->seq_ch.pan = 16;
    rsq->seq_ch.pitch = (32.7 * note_rates[rsq->last_klav_key] * (float) rsq->tune) / 32.0;
    /*
       fprintf(stderr, "pitch: %f\n", rsq->seq_ch.pitch);
     */
    rsq->seq_ch.accent = 0;
    rsq->last_klav_key = -1;
  }
  return PROC_SUCCESS;
}
