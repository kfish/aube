#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <X11/Xlib.h>
#include <glib.h>
#include <gtk/gtk.h>

#include "general_mixer.h"
#include "master.h"

#include "general_mixer_if.h"
#include "none_config.h"

extern int bpm, beat;
extern int tick;

int general_mixer_calc_note(general_mixer * xf);


module_class general_mixer_mif = {
  MIXER,
  0,
  (char *) "mixer",
  (char *) "General Mixer",
  NULL,
  (void *) general_mixer_new,
  (void *) general_mixer_clone,
  NULL,
  NULL,
  (void *) none_config_cb,
  (void *) general_mixer_if_new,
  (void *) general_mixer_if_close_cb
};

general_mixer *
general_mixer_new()
{
  general_mixer *xf;
  int i;

  xf = (general_mixer *) malloc(sizeof(general_mixer));
  xf->module.class = &general_mixer_mif;
  xf->module.on = 0;

  xf->module.nr_inputs = MAX_INPUTS;
  for (i = 0; i < MAX_INPUTS; i++) {
    xf->module.inputs[i] = malloc(sizeof(input));
    xf->module.inputs[i]->ch_type = CH_TYPE_AUDIO_STREAM;
    xf->module.inputs[i]->channel = NULL;
  }

  xf->module.nr_outputs = 1;
  xf->module.outputs[0] = malloc(sizeof(channel));
  xf->module.outputs[0]->parms.ch_type = CH_TYPE_AUDIO_STREAM;
  xf->module.outputs[0]->data = (bit16 *) (&(xf->tick_buffer));
  aube_module_clear_outputs((module *) xf);

  sprintf(xf->module.u_label, "mixer");		/*
						   not general_mixer for conciseness 
						 */
  sprintf(xf->module.outputs[0]->u_label, "out");
  xf->module.outputs[0]->module = (module *) xf;

  xf->module.do_tick = (void *) general_mixer_calc_note;

  for (i = 0; i < MAX_INPUTS; i++) {
    xf->vol[i] = 0;		/*
				   Initialise volume to zero 
				 */
    xf->mute[i] = 0;		/*
				   Initialise unmuted 
				 */
  }
  xf->nr_active_channels = 2;	/*
				   Init to 2 inputs 
				 */
  xf->master_vol = 64;

  aube_add_module((module *) xf);

  return xf;
}

general_mixer *
general_mixer_clone(general_mixer * oxf)
{
  general_mixer *xf;
  int i;

  xf = (general_mixer *) malloc(sizeof(general_mixer));
  memcpy(xf, oxf, sizeof(general_mixer));

  xf->module.nr_inputs = MAX_INPUTS;
  for (i = 0; i < MAX_INPUTS; i++) {
    xf->module.inputs[i] = malloc(sizeof(input));
    xf->module.inputs[i]->ch_type = CH_TYPE_AUDIO_STREAM;
    xf->module.inputs[i]->channel = oxf->module.inputs[i]->channel;
  }

  xf->module.nr_outputs = 1;
  xf->module.outputs[0] = malloc(sizeof(channel));
  xf->module.outputs[0]->parms.ch_type = CH_TYPE_AUDIO_STREAM;
  xf->module.outputs[0]->data = (bit16 *) (&(xf->tick_buffer));
  aube_module_clear_outputs((module *) xf);

  sprintf(xf->module.u_label, "mixer");
  sprintf(xf->module.outputs[0]->u_label, "out");
  xf->module.outputs[0]->module = (module *) xf;

  xf->module.do_tick = (void *) general_mixer_calc_note;

  aube_add_module((module *) xf);

  return xf;
}

int 
general_mixer_calc_note(general_mixer * xf)
{
  unsigned int i, j;
  int tmp;

  if (!xf->master_mute) {
    if (xf->nr_active_channels > 0) {
      for (i = 0; i < tick; i++) {
	tmp = 0;
	for (j = 0; j < xf->nr_active_channels; j++) {
	  if (xf->module.inputs[j]->channel && !xf->mute[j]) {
	    tmp += (int) (((bit16 *) (xf->module.inputs[j]->channel->data))[i] * xf->vol[j]);
	  }
	}
	xf->tick_buffer[i] = (bit16) ((tmp * xf->master_vol) / (64 * 64));
      }
    }
  } else {
    aube_module_clear_outputs((module *) xf);
  }

  return PROC_SUCCESS;
}


int 
general_mixer_add_input(general_mixer * gm)
{
  if (gm->nr_active_channels < MAX_INPUTS) {
    gm->nr_active_channels++;
    return (gm->nr_active_channels - 1);
  } else {
    return 0;
  }
}

/*
   returns index of removed input 
 */
int 
general_mixer_remove_input(general_mixer * gm)
{
  if (gm->nr_active_channels > 1) {
    gm->nr_active_channels--;
    return gm->nr_active_channels;
  } else {
    return 0;
  }
}
