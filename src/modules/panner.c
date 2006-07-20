#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <X11/Xlib.h>
#include <glib.h>
#include <gtk/gtk.h>

#include "panner.h"
#include "master.h"

#include "panner_if.h"
#include "none_config.h"

extern int bpm, beat;
extern int tick;

int panner_calc_note(panner * pn);

module_class panner_mif = {
  EFFECT,
  0,
  (char *) "panner",
  (char *) "Panner",
  NULL,
  (void *) panner_new,
  (void *) panner_clone,
  NULL,
  NULL,
  (void *) none_config_cb,
  (void *) panner_if_new,
  (void *) panner_if_close_cb
};

panner *
panner_new()
{
  panner *pn;

  pn = (panner *) malloc(sizeof(panner));
  pn->module.class = &panner_mif;
  pn->module.on = 0;
  pn->module.nr_inputs = 1;
  pn->module.inputs[0] = malloc(sizeof(input));
  pn->module.inputs[0]->ch_type = CH_TYPE_AUDIO_STREAM;
  pn->module.inputs[0]->channel = NULL;
  pn->module.nr_outputs = 1;
  pn->module.outputs[0] = malloc(sizeof(channel));
  pn->module.outputs[0]->parms.ch_type = CH_TYPE_AUDIO_STREAM;
  pn->module.outputs[0]->data = (bit16 *) (&(pn->tick_buffer));

  aube_module_clear_outputs((module *) pn);
  snprintf(pn->module.u_label, sizeof (pn->module.u_label), "panner");
  snprintf(pn->module.outputs[0]->u_label, sizeof (pn->module.outputs[0]->u_label), "out");
  pn->module.outputs[0]->module = (module *) pn;

  pn->module.do_tick = (void *) panner_calc_note;

  aube_add_module((module *) pn);

  pn->vol = 8;
  pn->pan = 16;

  return pn;
}

panner *
panner_clone(panner * opn)
{
  panner *pn;

  pn = (panner *) malloc(sizeof(panner));
  memcpy(pn, opn, sizeof(panner));

  pn->module.nr_inputs = 1;
  pn->module.inputs[0] = malloc(sizeof(input));
  pn->module.inputs[0]->ch_type = CH_TYPE_AUDIO_STREAM;
  pn->module.inputs[0]->channel = opn->module.inputs[0]->channel;
  pn->module.nr_outputs = 1;
  pn->module.outputs[0] = malloc(sizeof(channel));
  pn->module.outputs[0]->parms.ch_type = CH_TYPE_AUDIO_STREAM;
  pn->module.outputs[0]->data = (bit16 *) (&(pn->tick_buffer));

  aube_module_clear_outputs((module *) pn);
  snprintf(pn->module.u_label, sizeof (pn->module.u_label), "panner");
  snprintf(pn->module.outputs[0]->u_label, sizeof (pn->module.outputs[0]->u_label), "out");
  pn->module.outputs[0]->module = (module *) pn;

  pn->module.do_tick = (void *) panner_calc_note;

  aube_add_module((module *) pn);

  return pn;
}

int 
panner_calc_note(panner * pn)
{
  unsigned int i;
  int lpan, rpan;

  if (pn->pan <= 16) {
    lpan = pn->pan;
    rpan = 16;
  } else {
    lpan = 16;
    rpan = 31 - pn->pan;
  }

  for (i = 0; i < tick; i += 2) {

    pn->tick_buffer[i] =
      (bit16) ((((bit16 *) pn->module.inputs[0]->channel->data)[i] * lpan * pn->vol) >> 10);
    pn->tick_buffer[i + 1] =
      (bit16) ((((bit16 *) pn->module.inputs[0]->channel->data)[i + 1] * rpan * pn->vol) >> 10);
  }

  return PROC_SUCCESS;
}
