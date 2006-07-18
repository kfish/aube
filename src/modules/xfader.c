#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <X11/Xlib.h>
#include <glib.h>
#include <gtk/gtk.h>

#include "xfader.h"
#include "master.h"

#include "xfader_if.h"
#include "none_config.h"

extern int bpm, beat;
extern int tick;

int xfader_calc_note(xfader * xf);

module_class xfader_mif = {
  MIXER,
  0,
  (char *) "xfader",
  (char *) "XFader",
  NULL,
  (void *) xfader_new,
  (void *) xfader_clone,
  NULL,
  NULL,
  (void *) none_config_cb,
  (void *) xfader_if_new,
  (void *) xfader_if_close_cb
};

xfader *
xfader_new()
{
  xfader *xf;

  xf = (xfader *) malloc(sizeof(xfader));
  xf->module.class = &xfader_mif;
  xf->module.on = 0;
  xf->module.nr_inputs = 2;
  xf->module.inputs[0] = malloc(sizeof(input));
  xf->module.inputs[0]->ch_type = CH_TYPE_AUDIO_STREAM;
  xf->module.inputs[0]->channel = NULL;
  xf->module.inputs[1] = malloc(sizeof(input));
  xf->module.inputs[1]->ch_type = CH_TYPE_AUDIO_STREAM;
  xf->module.nr_outputs = 1;
  xf->module.outputs[0] = malloc(sizeof(channel));
  xf->module.outputs[0]->parms.ch_type = CH_TYPE_AUDIO_STREAM;
  xf->module.outputs[0]->data = (bit16 *) (&(xf->tick_buffer));
  /*
     memset(xf->module.outputs[0]->tick_buffer, 0, sizeof(xf->module.outputs[0]->tick_buffer));
   */
  aube_module_clear_outputs((module *) xf);
  snprintf(xf->module.u_label, sizeof (xf->module.u_label), "xfader");
  snprintf(xf->module.outputs[0]->u_label, sizeof (xf->module.outputs[0]->u_label), "out");
  xf->module.outputs[0]->module = (module *) xf;

  xf->module.do_tick = (void *) xfader_calc_note;

  aube_add_module((module *) xf);

  xf->vol0 = 64;
  xf->vol1 = 64;
  xf->fader = 32;

  return xf;
}

xfader *
xfader_clone(xfader * oxf)
{
  xfader *xf;

  xf = (xfader *) malloc(sizeof(xfader));
  memcpy(xf, oxf, sizeof(xfader));
  xf->module.nr_inputs = 2;
  xf->module.inputs[0] = malloc(sizeof(input));
  xf->module.inputs[0]->ch_type = CH_TYPE_AUDIO_STREAM;
  xf->module.inputs[0]->channel = oxf->module.inputs[0]->channel;
  xf->module.inputs[1] = malloc(sizeof(input));
  xf->module.inputs[1]->ch_type = CH_TYPE_AUDIO_STREAM;
  xf->module.inputs[1]->channel = oxf->module.inputs[1]->channel;
  xf->module.nr_outputs = 1;
  xf->module.outputs[0] = malloc(sizeof(channel));
  xf->module.outputs[0]->parms.ch_type = CH_TYPE_AUDIO_STREAM;
  xf->module.outputs[0]->data = (bit16 *) (&(xf->tick_buffer));
  aube_module_clear_outputs((module *) xf);
  snprintf(xf->module.u_label, sizeof (xf->module.u_label), "xfader");
  snprintf(xf->module.outputs[0]->u_label, sizeof (xf->module.outputs[0]->u_label), "out");
  xf->module.outputs[0]->module = (module *) xf;

  xf->module.do_tick = (void *) xfader_calc_note;

  aube_add_module((module *) xf);

  return xf;
}

int 
xfader_calc_note(xfader * xf)
{
  unsigned int i;

  if (xf->module.inputs[0]->channel == NULL && xf->module.inputs[1]->channel == NULL) {
    memset(xf->tick_buffer, 0, tick * 2);
  } else if (xf->module.inputs[0]->channel != NULL && xf->module.inputs[1]->channel == NULL) {
    memcpy(xf->tick_buffer, xf->module.inputs[0]->channel->data, tick);
  } else if (xf->module.inputs[0]->channel == NULL && xf->module.inputs[1]->channel != NULL) {
    memcpy(xf->tick_buffer, xf->module.inputs[1]->channel->data, tick);
  } else {
    for (i = 0; i < tick; i++) {
      xf->tick_buffer[i] =
	(bit16) (((((bit16 *) xf->module.inputs[0]->channel->data)[i] *
		   xf->vol0 * xf->fader) +
	    (((bit16 *) xf->module.inputs[1]->channel->data)[i] * xf->vol1 *
	     (64 - xf->fader))) >> 12);
    }
  }

  return PROC_SUCCESS;
}
