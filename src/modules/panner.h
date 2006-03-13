#ifndef __PANNER_H__
#define __PANNER_H__

#include "aube.h"

typedef struct {
  module module;
  bit16 tick_buffer[DEFAULT_TICK];
  int vol;
  int pan;
} panner;

panner *panner_new(void);
panner *panner_clone(panner * opn);

extern module_class panner_mif;

#endif /* __PANNER_H__  */
