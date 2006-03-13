#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <X11/Xlib.h>
#include <glib.h>
#include <gtk/gtk.h>

#include "panner4.h"
#include "master.h"

#include "panner4_if.h"
#include "none_config.h"

extern int bpm, beat;
extern int tick;

int panner4_calc_note(panner4 * pn);

module_class panner4_mif = {
  EFFECT,
  0,
  (char *) "panner4",
  (char *) "4way Panner",
  NULL,
  (void *) panner4_new,
  (void *) panner4_clone,
  NULL,
  NULL,
  (void *) none_config_cb,
  (void *) panner4_if_new,
  (void *) panner4_if_close_cb
};

panner4 *
panner4_new()
{
  panner4 *pn;

  pn = (panner4 *) malloc(sizeof(panner4));
  pn->module.class = &panner4_mif;
  pn->module.on = 0;
  pn->module.nr_inputs = 1;
  pn->module.inputs[0] = malloc(sizeof(input));
  pn->module.inputs[0]->ch_type = CH_TYPE_AUDIO_STREAM;
  pn->module.inputs[0]->channel = NULL;
  pn->module.nr_outputs = 2;
  pn->module.outputs[0] = malloc(sizeof(channel));
  pn->module.outputs[0]->parms.ch_type = CH_TYPE_AUDIO_STREAM;
  pn->module.outputs[0]->data = (bit16 *) (&(pn->fwd_buffer));
  pn->module.outputs[1] = malloc(sizeof(channel));
  pn->module.outputs[1]->parms.ch_type = CH_TYPE_AUDIO_STREAM;
  pn->module.outputs[1]->data = (bit16 *) (&(pn->rear_buffer));

  aube_module_clear_outputs((module *) pn);
  sprintf(pn->module.u_label, "panner4");
  sprintf(pn->module.outputs[0]->u_label, "fwd");
  pn->module.outputs[0]->module = (module *) pn;
  sprintf(pn->module.outputs[1]->u_label, "rear");
  pn->module.outputs[1]->module = (module *) pn;

  pn->module.do_tick = (void *) panner4_calc_note;

  aube_add_module((module *) pn);

  pn->vol = 64;
  pn->pan = 16;
  pn->fade = 16;

  return pn;
}

panner4 *
panner4_clone(panner4 * opn)
{
  panner4 *pn;

  pn = (panner4 *) malloc(sizeof(panner4));
  memcpy(pn, opn, sizeof(panner4));

  pn->module.nr_inputs = 1;
  pn->module.inputs[0] = malloc(sizeof(input));
  pn->module.inputs[0]->ch_type = CH_TYPE_AUDIO_STREAM;
  pn->module.inputs[0]->channel = opn->module.inputs[0]->channel;
  pn->module.nr_outputs = 2;
  pn->module.outputs[0] = malloc(sizeof(channel));
  pn->module.outputs[0]->parms.ch_type = CH_TYPE_AUDIO_STREAM;
  pn->module.outputs[0]->data = (bit16 *) (&(pn->fwd_buffer));
  pn->module.outputs[1] = malloc(sizeof(channel));
  pn->module.outputs[1]->parms.ch_type = CH_TYPE_AUDIO_STREAM;
  pn->module.outputs[1]->data = (bit16 *) (&(pn->rear_buffer));

  aube_module_clear_outputs((module *) pn);
  sprintf(pn->module.u_label, "panner4");
  sprintf(pn->module.outputs[0]->u_label, "fwd");
  pn->module.outputs[0]->module = (module *) pn;
  sprintf(pn->module.outputs[1]->u_label, "rear");
  pn->module.outputs[1]->module = (module *) pn;

  pn->module.do_tick = (void *) panner4_calc_note;

  aube_add_module((module *) pn);

  return pn;
}

int 
panner4_calc_note(panner4 * pn)
{
  unsigned int i;
  int lpan, rpan;
  int ffade, rfade;
  bit16 r, l;

  if (pn->pan <= 16) {
    lpan = pn->pan;
    rpan = 16;
  } else {
    lpan = 16;
    rpan = 31 - pn->pan;
  }

  if (pn->fade <= 16) {
    ffade = pn->fade;
    rfade = 16;
  } else {
    ffade = 16;
    rfade = 31 - pn->fade;
  }

  for (i = 0; i < tick; i += 2) {
    l = (bit16) ((((bit16 *) pn->module.inputs[0]->channel->data)[i] * lpan) >> 4);
    r = (bit16) ((((bit16 *) pn->module.inputs[0]->channel->data)[i + 1] * rpan) >> 4);
    pn->fwd_buffer[i] = (l * ffade) >> 4;
    pn->fwd_buffer[i + 1] = (r * ffade) >> 4;
    pn->rear_buffer[i] = (l * rfade) >> 4;
    pn->rear_buffer[i + 1] = (r * rfade) >> 4;
  }

  return PROC_SUCCESS;
}
