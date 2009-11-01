
#ifndef __NONE_H__
#define __NONE_H__

#include "aube.h"

typedef struct {
	module module;
	bit16 zero_buffer[DEFAULT_TICK];
	seq_channel seq_ch;
} none;

none *none_new(void);

#endif				/*
				   __NONE_H__  
				 */
