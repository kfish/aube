#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <X11/Xlib.h>
#include <glib.h>
#include <gtk/gtk.h>

#include "none.h"
#include "master.h"

extern int bpm, beat;
extern int tick;

int none_calc_note(none * wn);

module_class none_mif =
{
  -1,
  0,
  (char *) "none",
  (char *) "None",
  NULL,
  (void *) none_new,
  NULL,
  NULL,
  NULL,
  NULL
};

none *
none_new()
{
  none *nn;

  nn = (none *) malloc(sizeof(none));
  nn->module.class = &none_mif;
  nn->module.nr_inputs = 0;
  nn->module.nr_outputs = 2;
  nn->module.outputs[0] = malloc(sizeof(channel));
  nn->module.outputs[0]->parms.ch_type = CH_TYPE_AUDIO_STREAM;
  nn->module.outputs[0]->data = (bit16 *) (&(nn->zero_buffer));
  nn->module.outputs[1] = malloc(sizeof(channel));
  nn->module.outputs[1]->parms.ch_type = CH_TYPE_SEQUENCE;
  nn->module.outputs[1]->data = (seq_channel *) (&(nn->seq_ch));
  aube_module_clear_outputs((module *) nn);

  sprintf(nn->module.u_label, "None");
  sprintf(nn->module.outputs[0]->u_label, "out");
  nn->module.outputs[0]->module = (module *) nn;
  sprintf(nn->module.outputs[1]->u_label, "seq");
  nn->module.outputs[1]->module = (module *) nn;

  nn->module.do_tick = (void *) none_calc_note;

  aube_add_module((module *) nn);

  return nn;
}


int
none_calc_note(none * nn)
{
  return PROC_SUCCESS;
}
