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

#ifndef _OSS_H_
#define _OSS_H_

#include "aube.h"

#define DEVICENAME_LEN 64
#define DEFAULT_DEVICE (char *)"/dev/dsp"
#define DEFAULT_FORMAT AFMT_S16_LE
#define DEFAULT_SAMPLESIZE 8
#define DEFAULT_STEREO 1
#define DEFAULT_FREQUENCY 44100
#define DEFAULT_NUMFRAGS 8
#define DEFAULT_FRAGSIZE 17
#define OSS_BUFFER_TICKS 16

#define OSS_MODE_NONE   0
#define OSS_MODE_OUTPUT 1
#define OSS_MODE_INPUT  2
#define OSS_MODE_DUPLEX 3

typedef struct _oss_out oss;
typedef struct _oss_dev oss_dev;
typedef struct _oss_out oss_out;
typedef struct _oss_in oss_in;

struct _oss_dev {
	int file;
	char devicename[DEVICENAME_LEN];
	module *reader;		/* a line-in module */
	module *writer;		/* a line-out module */
	int mode;
	int mask;
	int format;
	int stereo;
	int frequency;
	int numfrags;
	int fragsize;
};

struct _oss_out {
	module output_module;
	oss_dev *dev;
};

struct _oss_in {
	module input_module;
	oss_dev *dev;
	bit16 tick_buffer[2 * DEFAULT_FREQUENCY];	/* one second of sound */
};

oss_out *oss_out_new(void);
oss_in *oss_in_new(void);
void oss_open(oss_dev * oss_d);
void oss_close(oss_dev * oss_d);
void oss_set_mode(oss_dev * oss_d, int mode);
void lineout_set_devicename(oss_out * oss_o, char *devname);
void linein_set_devicename(oss_in * oss_i, char *devname);
int oss_dev_add_writer(oss_dev * oss_d, oss_out * oss_o);
int oss_dev_add_reader(oss_dev * oss_d, oss_in * oss_i);
int oss_dev_remove_writer(oss_dev * oss_d);
int oss_dev_remove_reader(oss_dev * oss_d);

extern module_class linein_mif;
extern module_class lineout_mif;

#endif				/* _OSS_H_ */
