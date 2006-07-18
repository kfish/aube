#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/soundcard.h>
#include <sys/time.h>
#include <sys/types.h>
#include <math.h>
#include <errno.h>

#include "aube.h"
#include "line.h"

#include "linein_if.h"
#include "lineout_if.h"

#include "none_config.h"

extern int tick;

int lineout_do_tick(oss_out * oss_p);
int linein_do_tick(oss_in * oss_p);
int lineout_off(module  *oss_o);
int lineout_on(oss_out *oss_o);

void oss_set_format(oss_dev * oss_d);
void oss_set_stereo(oss_dev * oss_d);
void oss_set_frequency(oss_dev * oss_d);
void oss_set_numfrags(oss_dev * oss_d);
void oss_set_devicename(oss * oss_p, char *devname);


module_class lineout_mif =
{
  OUTPUT,
  0,
  (char *) "line-out",
  (char *) "Line Out",
  NULL,
  (void *) oss_out_new,
  NULL,
  lineout_off,  /* off */
  lineout_on,  /* on */
  (void *) none_config_cb,
  (void *) lineout_if_new,
  (void *) lineout_if_close_cb
};

module_class linein_mif =
{
  GENERATOR,
  0,
  (char *) "line-in",
  (char *) "Line In",
  NULL,
  (void *) oss_in_new,
  NULL,
  NULL,  /* off */
  NULL,  /* on */
  (void *) none_config_cb,
  (void *) linein_if_new,
  (void *) linein_if_close_cb
};

struct timeval tv_instant =
{0, 0};

static oss_dev *oss_dev_tbl[2] =
{NULL, NULL};

int 
oss_setup_dev_tbl(void)
{
  int i;
  oss_dev *oss_d;

  for (i = 0; i < 2; i++) {
    oss_d = (oss_dev *) malloc(sizeof(oss_dev));
    oss_d->file = -1;
    oss_d->mode = OSS_MODE_OUTPUT;
    /* strncpy(oss_d->devicename, DEFAULT_DEVICE, DEVICENAME_LEN);*/
    if (i == 0) {
      strncpy(oss_d->devicename, "/dev/dsp", DEVICENAME_LEN);
    } else {
      strncpy(oss_d->devicename, "/dev/dsp1", DEVICENAME_LEN);
    }
    oss_d->mask = 0;
    oss_d->format = DEFAULT_FORMAT;
    oss_d->stereo = DEFAULT_STEREO;
    oss_d->frequency = DEFAULT_FREQUENCY;
    oss_d->numfrags = DEFAULT_NUMFRAGS;
    oss_d->fragsize = DEFAULT_FRAGSIZE;

    oss_d->reader = NULL;
    oss_d->writer = NULL;

    oss_dev_tbl[i] = oss_d;
  }

  return 1;
}

oss_out *
oss_out_new()
{
  oss_out *oss_o;
  int dev_tbl_i;

  if (oss_dev_tbl[0] == NULL) {
    oss_setup_dev_tbl();
  }
  if (oss_dev_tbl[0]->writer == NULL) {
    dev_tbl_i = 0;
  } else if (oss_dev_tbl[1]->writer == NULL) {
    dev_tbl_i = 1;
  } else {
    aube_error(AUBE_ERROR_RECOVERABLE, "OSS Out: No devices left!!");
    return NULL;
  }

  oss_o = (oss_out *) malloc(sizeof(oss_out));

  /* output_module does output to the device */
  oss_o->output_module.do_tick = (void *) lineout_do_tick;
  oss_o->output_module.class = &lineout_mif;
  oss_o->output_module.on = 0;
  oss_o->output_module.nr_inputs = 1;
  oss_o->output_module.inputs[0] = malloc(sizeof(input));
  oss_o->output_module.inputs[0]->ch_type = CH_TYPE_AUDIO_STREAM;
  oss_o->output_module.inputs[0]->channel = NULL;
  oss_o->output_module.nr_outputs = 0;
  snprintf(oss_o->output_module.u_label, sizeof (oss_o->output_module.u_label), "Line-Out");
  aube_add_module(&oss_o->output_module);

  oss_o->dev = oss_dev_tbl[dev_tbl_i];

/* oss_dev_add_writer(oss_dev_tbl[dev_tbl_i], oss_o); */

  return oss_o;
}

oss_in *
oss_in_new()
{
  oss_in *oss_i;
  int dev_tbl_i;

  if (oss_dev_tbl[0] == NULL) {
    oss_setup_dev_tbl();
  }
  if (oss_dev_tbl[0]->reader == NULL) {
    dev_tbl_i = 0;
  } else if (oss_dev_tbl[1]->reader == NULL) {
    dev_tbl_i = 1;
  } else {
    aube_error(AUBE_ERROR_RECOVERABLE, "OSS In: No devices left!!");
    return NULL;
  }

  oss_i = (oss_in *) malloc(sizeof(oss_in));

  /*
     input_module takes input from the device 
   */
  oss_i->input_module.do_tick = (void *) linein_do_tick;
  oss_i->input_module.class = &linein_mif;
  oss_i->input_module.on = 0;
  oss_i->input_module.nr_inputs = 0;
  oss_i->input_module.nr_outputs = 1;
  oss_i->input_module.outputs[0] = malloc(sizeof(channel));
  oss_i->input_module.outputs[0]->parms.ch_type = CH_TYPE_AUDIO_STREAM;
  oss_i->input_module.outputs[0]->data = (bit16 *) (&oss_i->tick_buffer);
  aube_module_clear_outputs(&oss_i->input_module);
  snprintf(oss_i->input_module.u_label, sizeof (oss_i->input_module.u_label), "Line-In");
  snprintf(oss_i->input_module.outputs[0]->u_label, sizeof (oss_i->input_module.outputs[0]->u_label), "line");
  oss_i->input_module.outputs[0]->module = &oss_i->input_module;
  aube_add_module(&oss_i->input_module);

  oss_i->dev = oss_dev_tbl[dev_tbl_i];

/*
  oss_dev_add_reader(oss_dev_tbl[dev_tbl_i], oss_i);
*/

  return oss_i;
}


int 
lineout_do_tick(oss_out * oss_p)
{
  int n, ret;
  char buf[64];
  bit8 *b = NULL;
  fd_set fds;

  if (!((oss_p->dev->mode & OSS_MODE_OUTPUT) && oss_p->output_module.on)) {
#ifdef DEBUG
    printf ("OSS lineout_do_tick: not ready / not on\n");
#endif
    return PROC_FAILURE;
  }
  if(oss_p->dev->file == -1) {
#ifdef DEBUG
    printf ("OSS: reopening device\n");
#endif
    oss_open(oss_p->dev);
  }

#if 1
  FD_ZERO(&fds);
  FD_SET(oss_p->dev->file, &fds);

  if ((ret = select(oss_p->dev->file + 1, NULL, &fds, NULL, &tv_instant)) == 0) {    
#ifdef DEBUG
    printf ("OSS PROC_INCOMPLETE: fd %d\tret = %d\n", oss_p->dev->file, ret);
#endif
    /*    return PROC_INCOMPLETE;*/
  }
#endif

  if (oss_p->dev->format & AFMT_S16_LE) {
    if ((oss_p->dev->mode == OSS_MODE_OUTPUT) && oss_p->output_module.on) {
      if (oss_p->dev->file != -1 && ((module *) oss_p)->inputs[0] != NULL &&
	  ((module *) oss_p)->inputs[0]->channel != NULL &&
	  ((module *) oss_p)->inputs[0]->channel->module->on) {
	if ((n = write(oss_p->dev->file,
		       ((module *) oss_p)->inputs[0]->channel->data,
		       tick * 2)) == -1) {
#if 1
	  if (errno != 11) {
#endif

	    snprintf(buf, sizeof (buf), "OSS: Error writing to %s (%d)",
		    oss_p->dev->devicename, errno);
	    perror(buf);

#if 1
	  }
#endif
	  return PROC_INCOMPLETE;

#ifdef DEBUG
	} else {
	  fprintf(stderr, "%d bytes written to fd %d (%s)\n", n,
		  oss_p->dev->file, oss_p->dev->devicename);
#endif
	}
      }
    }
  } else if (oss_p->dev->format & AFMT_U8) {
    if (!b) {
      b = (bit8 *) malloc(tick);
    }
    for (n = 0; n < tick; n++) {
      *(b + n) = (((bit16 *) (((module *) oss_p)->inputs[0]->channel->data))[n]) >> 8;
    }
    if ((n = write(oss_p->dev->file, b, tick)) == -1) {
      snprintf(buf, sizeof (buf), "OSS: Error writing to %s", oss_p->dev->devicename);
      perror(buf);
#ifdef DEBUG
    } else {
      fprintf(stderr, "%d bytes written to fd %d (%s)\n", n, oss_p->dev->file,
	      oss_p->dev->devicename);
#endif
    }
  }
  return PROC_SUCCESS;
}

int 
linein_do_tick(oss_in * oss_p)
{
  int ret;
  fd_set fds;

  if ((oss_p->dev->mode & OSS_MODE_INPUT) && oss_p->input_module.on) {

    if(oss_p->dev->file == -1) {
      oss_open(oss_p->dev);
    }

    FD_ZERO(&fds);
    FD_SET(oss_p->dev->file, &fds);

    if ((ret = select(oss_p->dev->file + 1, &fds, NULL, NULL, &tv_instant)) == 0) {
      return PROC_INCOMPLETE;
    }
    if (ret > 0 && oss_p->dev->file != -1) {
      read(oss_p->dev->file, oss_p->tick_buffer, tick * 2);
    }
  } else {
    memset(oss_p->tick_buffer, 0, tick * 2);
  }

  return PROC_SUCCESS;
}

int
lineout_off(module  *oss_o)
{
  return oss_dev_remove_writer(((oss_out *)oss_o)->dev);
}

int
lineout_on(oss_out *oss_o)
{
  return oss_dev_add_writer(((oss_out *)oss_o)->dev, oss_o);
}

#define MODULE_ON_safe(m)       if(m) m->on = 1;
#define MODULE_OFF_safe(m)      if(m) m->on = 0;

#ifdef DEBUG
static void
oss_dump_settings (oss_dev * oss_d)
{
  printf ("%s:\n", oss_d->devicename);
  printf ("\t%s reader\n", oss_d->reader ? "Has" : "NO");
  printf ("\t%s writer\n", oss_d->writer ? "Has" : "NO");
  printf ("\tformat: ");
  switch (oss_d->format) {
  case AFMT_U8: printf ("AFMT_U8"); break;
  case AFMT_U16_LE: printf ("AFMT_U16_LE"); break;
  case AFMT_S16_LE: printf ("AFMT_S16_LE"); break;
  case AFMT_U16_BE: printf ("AFMT_U16_BE"); break;
  case AFMT_S16_BE: printf ("AFMT_S16_BE"); break;
  case AFMT_S8: printf ("AFMT_S8"); break;
  default: printf ("Unknown %x\n", oss_d->format);
  }
  printf ("\t%s\n", oss_d->stereo ? "Stereo" : "Mono");
  printf ("\tfrequency %d\n", oss_d->frequency);
  printf ("\tnumfrags: %d\n", oss_d->numfrags);
  printf ("\tfragsize: %d\n", oss_d->fragsize);
}
#endif

void 
oss_open(oss_dev * oss_d)
{
  char buf[64];

  switch (oss_d->mode) {
  case OSS_MODE_NONE:
    MODULE_OFF_safe(oss_d->writer);
    MODULE_OFF_safe(oss_d->reader);
    return;
    break;
  case OSS_MODE_OUTPUT:

    if ((oss_d->file = open(oss_d->devicename, O_WRONLY, 0)) == -1) {
      snprintf(buf, sizeof (buf), "OSS: Unable to open %s for writing", oss_d->devicename);
      perror(buf);
      MODULE_OFF_safe(oss_d->writer);
      MODULE_OFF_safe(oss_d->reader);
      return;
    } else {

#ifdef DEBUG
      snprintf(buf, sizeof (buf), "OSS: opened %s for writing, fd %d",
	      oss_d->devicename, oss_d->file);
      perror(buf);
#endif
      MODULE_ON_safe(oss_d->writer);
      MODULE_ON_safe(oss_d->reader);
    }
    break;
  case OSS_MODE_INPUT:

    if ((oss_d->file = open(oss_d->devicename, O_RDONLY | O_NDELAY, 0)) == -1) {
      snprintf(buf, sizeof (buf), "OSS: Unable to open %s for reading", oss_d->devicename);
      perror(buf);
      MODULE_OFF_safe(oss_d->writer);
      MODULE_OFF_safe(oss_d->reader);
      return;
    } else {
#ifdef DEBUG
      snprintf(buf, sizeof (buf), "OSS: opened %s for reading, fd %d",
	      oss_d->devicename, oss_d->file);
      perror(buf);
#endif
      MODULE_ON_safe(oss_d->writer);
      MODULE_ON_safe(oss_d->reader);
    }
    break;
  case OSS_MODE_DUPLEX:

    if ((oss_d->file = open(oss_d->devicename, O_RDWR | O_NDELAY, 0)) == -1) {
      snprintf(buf, sizeof (buf), "OSS: Unable to open %s for duplex", oss_d->devicename);
      perror(buf);
      MODULE_OFF_safe(oss_d->writer);
      MODULE_OFF_safe(oss_d->reader);
      return;
    } else {
#ifdef DEBUG
      snprintf(buf, sizeof (buf), "OSS: opened %s for duplex, fd %d",
	      oss_d->devicename, oss_d->file);
      perror(buf);
#endif
      MODULE_ON_safe(oss_d->writer);
      MODULE_ON_safe(oss_d->reader);
    }
    break;
  default:
    break;
  }

  /*
   * At this point, we have ensured oss_d->file != -1, and refers to a
   * valid file which we have just opened.
   */

  {
    int stereo = 0;
    int temp;
    int channels;
    int srate = 44100;
    int error;

    if (ioctl (oss_d->file, SNDCTL_DSP_STEREO, &stereo) == -1)
      {       /* Fatal error */
	perror("open_dsp_device 2 ") ;
	exit (1);
      } ;
    
    if (ioctl (oss_d->file, SNDCTL_DSP_RESET, 0))
      {       perror ("open_dsp_device 3 ") ;
      exit (1) ;
      } ;
    
    temp = 16 ;
    if ((error = ioctl (oss_d->file, SOUND_PCM_WRITE_BITS, &temp)) != 0)
      {       perror ("open_dsp_device 4 ") ;
      exit (1) ;
      } ;
    
    channels = 2;
    if ((error = ioctl (oss_d->file, SOUND_PCM_WRITE_CHANNELS, &channels)) != 0)
      {       perror ("open_dsp_device 5 ") ;
      exit (1) ;
      } ;
    
    if ((error = ioctl (oss_d->file, SOUND_PCM_WRITE_RATE, &srate)) != 0)
      {       perror ("open_dsp_device 6 ") ;
      exit (1) ;
      } ;
    
    if ((error = ioctl (oss_d->file, SNDCTL_DSP_SYNC, 0)) != 0)
      {       perror ("open_dsp_device 7 ") ;
      exit (1) ;
      } ;
    
  }

#if 0
  {
    int stereo = 1;
    int size = AFMT_S16_LE;
    int speed = 44100;


    ioctl (oss_d->file, SNDCTL_DSP_SAMPLESIZE, &size);
    ioctl (oss_d->file, SNDCTL_DSP_STEREO, &stereo);
    ioctl (oss_d->file, SNDCTL_DSP_SPEED, &speed);
  }
#endif

#if 0

  oss_set_stereo(oss_d);


  if (ioctl(oss_d->file, SNDCTL_DSP_GETFMTS, &oss_d->mask) == -1) {
    perror("OSS: error getting format masks");
    close(oss_d->file);
    MODULE_OFF_safe(oss_d->writer);
    MODULE_OFF_safe(oss_d->reader);
    return;
  }
  if (oss_d->mask & AFMT_U8) {
    oss_d->format = AFMT_U8;
  }
  if (oss_d->mask & AFMT_U16_LE) {
    oss_d->format = AFMT_U16_LE;
  }
  if (oss_d->mask & AFMT_S16_LE) {
    oss_d->format = AFMT_S16_LE;
  }
  if (oss_d->mask & AFMT_U16_BE) {
    oss_d->format = AFMT_U16_BE;
  }
  if (oss_d->mask & AFMT_S16_BE) {
    oss_d->format = AFMT_S16_BE;
  }
  if (oss_d->mask & AFMT_S8) {
    oss_d->format = AFMT_S8;
  }
  if (oss_d->mask & AFMT_S16_LE) {
    oss_d->format = AFMT_S16_LE;
  }
  oss_set_format(oss_d);

  oss_set_frequency(oss_d);
  oss_set_numfrags(oss_d);
#endif

#ifdef DEBUG
  oss_dump_settings (oss_d);
#endif
}

void 
oss_set_format(oss_dev * oss_d)
{
  if (ioctl(oss_d->file, SNDCTL_DSP_SETFMT, &oss_d->format) == -1) {
    perror("OSS: Unable to set format");
    exit(-1);
  }
}

void 
oss_set_stereo(oss_dev * oss_d)
{
  if (ioctl(oss_d->file, SNDCTL_DSP_STEREO, &(oss_d->stereo)) == -1) {
    perror("OSS: Unable to set channels");
  }
}

void 
oss_set_frequency(oss_dev * oss_d)
{
  if (ioctl(oss_d->file, SNDCTL_DSP_SPEED, &(oss_d->frequency)) == -1) {
    perror("OSS: Unable to set playback frequency");
  }
}

void 
oss_set_numfrags(oss_dev * oss_d)
{
  int fragmentsize;

  fragmentsize = (oss_d->numfrags << 16) | oss_d->fragsize;
  if (ioctl(oss_d->file, SNDCTL_DSP_SETFRAGMENT, &fragmentsize) == -1) {
    perror("OSS: Unable to set buffer fragment");
  }
}

void 
oss_close(oss_dev * oss_d)
{
  if(oss_d->file != -1) close(oss_d->file);
  MODULE_OFF_safe(oss_d->writer);
  MODULE_OFF_safe(oss_d->reader);
}

int 
oss_dev_add_writer(oss_dev * oss_d, oss_out * oss_o)
{
  oss_d->writer = &oss_o->output_module;
  oss_o->dev = oss_d;
  oss_d->mode |= OSS_MODE_OUTPUT;

  oss_close(oss_d);
  oss_open(oss_d);

  return 1;
}

int 
oss_dev_add_reader(oss_dev * oss_d, oss_in * oss_i)
{
  oss_d->reader = &oss_i->input_module;
  oss_i->dev = oss_d;
  oss_d->mode |= OSS_MODE_INPUT;

  oss_close(oss_d);
  oss_open(oss_d);
  return 1;
}

int 
oss_dev_remove_writer(oss_dev * oss_d)
{
  if (oss_d->writer) {
    oss_d->writer = NULL;
    if (oss_d->reader) {
      oss_d->mode = OSS_MODE_INPUT;
    } else {
      oss_d->mode = OSS_MODE_NONE;
    }
    oss_close(oss_d);
    oss_open(oss_d);
    return 1;
  } else {
    return 0;
  }
}

int 
oss_dev_remove_reader(oss_dev * oss_d)
{
  if (oss_d->reader) {
    oss_d->reader = NULL;
    if (oss_d->writer) {
      oss_d->mode = OSS_MODE_OUTPUT;
    } else {
      oss_d->mode = OSS_MODE_NONE;
    }
    oss_close(oss_d);
    oss_open(oss_d);
    return 1;
  } else {
    return 0;
  }
}


void 
lineout_set_devicename(oss_out * oss_o, char *devname)
{
  int i;

  /*
     First we check that we're not asking for the device we already have 
   */
  if (!strncmp(oss_o->dev->devicename, devname, DEVICENAME_LEN)) {
    return;
  }
  for (i = 0; i < 2; i++) {
    if (!strncmp(oss_dev_tbl[i]->devicename, devname, DEVICENAME_LEN)) {
      if (oss_dev_tbl[i]->writer == NULL) {

	/*
	   Fix up old device 
	 */
	oss_dev_remove_writer(oss_o->dev);

	/*
	   Set up the new 
	 */
	oss_dev_add_writer(oss_dev_tbl[i], oss_o);

	return;
      } else {
	aube_error(AUBE_ERROR_RECOVERABLE, "OSS Out: Device in use");
      }
    }
  }
}

void 
linein_set_devicename(oss_in * oss_i, char *devname)
{
  int i;

  /*
     First we check that we're not asking for the device we already have 
   */
  if (!strncmp(oss_i->dev->devicename, devname, DEVICENAME_LEN)) {
    return;
  }
  for (i = 0; i < 2; i++) {
    if (!strncmp(oss_dev_tbl[i]->devicename, devname, DEVICENAME_LEN)) {
      if (oss_dev_tbl[i]->reader == NULL) {

	/*
	   Fix up old device 
	 */
	oss_dev_remove_reader(oss_i->dev);

	/*
	   Set up the new 
	 */
	oss_dev_add_reader(oss_dev_tbl[i], oss_i);

	return;
      } else {
	aube_error(AUBE_ERROR_RECOVERABLE, "OSS In: Device in use");
      }
    }
  }
}
