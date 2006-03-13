#ifndef __PANNER4_H__
#define __PANNER4_H__

#include "aube.h"

typedef struct {
  module module;
  bit16 fwd_buffer[DEFAULT_TICK];
  bit16 rear_buffer[DEFAULT_TICK];
  int vol;
  int pan;
  int fade;
} panner4;

panner4 *panner4_new(void);
panner4 *panner4_clone(panner4 * opn);

extern module_class panner4_mif;

#endif /* __PANNER4_H__  */
