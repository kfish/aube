#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <config.h>

#ifdef USE_SGI_HEADERS
#include <dmedia/audiofile.h>
#else
#include <audiofile.h>
#endif

#include "aube.h"
#include "sample.h"

int
sample_load(char *filename)
{
#ifdef HAVE_LIBAUDIOFILE
  AFframecount framecount;
  AFfilehandle samplefile;
  float framesize;
  int channelcount, sampleformat, samplewidth;
  void *data;
  channel *ch;
  char *fn, *pfn;
  int fn_len;
  int i;

  samplefile = afOpenFile(filename, "r", NULL);
  if (samplefile <= 0)
    return (int) samplefile;

  framecount = afGetFrameCount(samplefile, AF_DEFAULT_TRACK);
  if (framecount <= 0)
    return 0;
  channelcount = afGetChannels(samplefile, AF_DEFAULT_TRACK);
  if (channelcount <= 0)
    return 0;
  afGetSampleFormat(samplefile, AF_DEFAULT_TRACK, &sampleformat, &samplewidth);

#if defined(i386) || defined(alpha)
  afSetVirtualByteOrder(samplefile, AF_DEFAULT_TRACK, AF_BYTEORDER_LITTLEENDIAN);
#else
  afSetVirtualByteOrder(samplefile, AF_DEFAULT_TRACK, AF_BYTEORDER_BIGENDIAN);
#endif

  if (samplewidth != 16 && samplewidth != 8) {
    aube_error(AUBE_ERROR_RECOVERABLE, "Sample width unsupported\n");
    return 0;
  }
  framesize = afGetFrameSize(samplefile, AF_DEFAULT_TRACK, 0);

  ch = malloc(sizeof(channel));
  ch->parms.d.audio_sample_data.s_length = framecount * framesize / channelcount;
  ch->module = NULL;

  data = malloc(framecount * (samplewidth / 8) * channelcount);
  afReadFrames(samplefile, AF_DEFAULT_TRACK, data, framecount);

  if (samplewidth == 16 && channelcount == 2) {
    ch->data = data;
  } else {
    ch->data = malloc(framecount * 2 * 2);
    if (ch->data == NULL) {
      aube_error(AUBE_ERROR_RECOVERABLE, "ch->data NULL");
      return 0;
    }
    if (samplewidth == 16 && channelcount == 1) {
      for (i = 0; i < (framecount * (samplewidth / 8) / 2); i++) {
	((bit16 *) ch->data)[2 * i + 1] = ((bit16 *) ch->data)[2 * i] = ((bit16 *) data)[i];
      }
    } else if (samplewidth == 8 && channelcount == 2) {
      for (i = 0; i < (framecount * (samplewidth / 8)); i++) {
	((bit16 *) ch->data)[i] = (((bit8 *) data)[i]) << 8;

      }
    } else if (samplewidth == 8 && channelcount == 1) {
      bit16 w;
      for (i = 0; i < framecount; i++) {
	w = (bit16) (((bit8 *) data)[i]);
	((bit16 *) ch->data)[2 * i + 1] = ((bit16 *) ch->data)[2 * i] = w << 8;
	/*
	   ((bit16 *)ch->data)[2*i+1] = ((bit16 *)ch->data)[2*i] = (((bit8 *)data)[i]) << 8;
	 */
      }
    }
    free(data);
  }

  afCloseFile(samplefile);

  /*
     strip leading path from filename 
   */
  pfn = strtok(filename, "/");
  while ((fn = strtok(NULL, "/")))
    pfn = fn;
  fn_len = strlen(pfn) + 1;
  strncpy(ch->u_label, pfn, MIN(fn_len, LABEL_LEN));

  return aube_add_sample(ch);
#endif
}

int
sample_save(channel * channel, char *filename)
{
#ifdef HAVE_LIBAUDIOFILE
  AFframecount frameCount;
  AFfilehandle outputFile;
  AFfilesetup outputSetup;
  float framesize;
  int format;

  /*
     Hardcoded ;) 
   */
  format = AF_FILE_WAVE;

  outputSetup = afNewFileSetup();
  afInitFileFormat(outputSetup, format);
  afInitRate(outputSetup, AF_DEFAULT_TRACK, 44100);
  afInitChannels(outputSetup, AF_DEFAULT_TRACK, 2);
  outputFile = afOpenFile(filename, "w", outputSetup);
  afFreeFileSetup(outputSetup);

  afSetVirtualByteOrder(outputFile, AF_DEFAULT_TRACK, AF_BYTEORDER_LITTLEENDIAN);

  framesize = 4;
  frameCount = channel->parms.d.audio_sample_data.s_length * 2 / framesize;

  afWriteFrames(outputFile, AF_DEFAULT_TRACK, channel->data, frameCount);
  afCloseFile(outputFile);

  return 0;
#endif
}
