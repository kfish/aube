/*
 * AUBE/Metadecks Live
 *
 * Copyright (C) 1998-2009 Conrad Parker
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <libgen.h>

#include <config.h>

#ifdef HAVE_LIBSNDFILE
#include <sndfile.h>
#endif

#include "aube.h"
#include "sample.h"

int sample_load(const char *filename)
{
#ifdef HAVE_LIBSNDFILE
	sf_count_t framecount;
	SNDFILE *samplefile;
	SF_INFO sfinfo;
	int channelcount, samplewidth;
	void *data;
	channel *ch;
	char *labelname;
	int labellen;
	int i;


	memset(&sfinfo, 0, sizeof(sfinfo));
	samplefile = sf_open(filename, SFM_READ, &sfinfo);
	if (samplefile <= 0)
		return 0;

	framecount = sfinfo.frames;
	if (framecount <= 0)
		return 0;
	channelcount = sfinfo.channels;
	if (channelcount <= 0)
		return 0;

	/* Default to 16 bit PCM samples. */
	samplewidth = 16;

	ch = malloc(sizeof(channel));
	ch->parms.d.audio_sample_data.s_length =
	    framecount * (samplewidth / 8) / channelcount;
	ch->module = NULL;

	data = malloc(framecount * (samplewidth / 8) * channelcount);
	sf_readf_short(samplefile, data, framecount);

	if (samplewidth == 16 && channelcount == 2) {
		ch->data = data;
	} else {
		ch->data = malloc(framecount * 2 * 2);
		if (ch->data == NULL) {
			aube_error(AUBE_ERROR_RECOVERABLE,
				   "ch->data NULL");
			return 0;
		}
		if (samplewidth == 16 && channelcount == 1) {
			for (i = 0;
			     i < (framecount * (samplewidth / 8) / 2);
			     i++) {
				((bit16 *) ch->data)[2 * i + 1] =
				    ((bit16 *) ch->data)[2 * i] =
				    ((bit16 *) data)[i];
			}
		}
		free(data);
	}

	sf_close(samplefile);

	/* strip leading path from filename */
        labelname = basename ((char *)filename);
	labellen = strlen(labelname) + 1;
	strncpy(ch->u_label, labelname, MIN(labellen, LABEL_LEN));

	return aube_add_sample(ch);
#endif
}

int sample_save(channel * channel, const char *filename)
{
#ifdef HAVE_LIBSNDFILE
	sf_count_t frameCount;
	SNDFILE *outputFile;
	SF_INFO sfinfo;
	float framesize;

	/*
	   Hardcoded ;) 
	 */
	sfinfo.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;
	sfinfo.channels = 2;
	sfinfo.samplerate = 44100;

	outputFile = sf_open(filename, SFM_WRITE, &sfinfo);
	if (outputFile == NULL)
		return 0;

	framesize = sizeof(short) * sfinfo.channels;
	frameCount =
	    channel->parms.d.audio_sample_data.s_length * 2 / framesize;

	sf_writef_short(outputFile, channel->data, frameCount);
	sf_close(outputFile);

	return 0;
#endif
}
