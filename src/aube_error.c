#include <stdio.h>
#include "aube.h"

void
aube_error(int errlvl, char *msg)
{
  switch (errlvl) {
  case AUBE_MESSAGE:
    fprintf(stderr, "AUBE: %s\n", msg);
    break;
  case AUBE_ERROR_RECOVERABLE:
    fprintf(stderr, "AUBE: %s\n", msg);
    break;
  case AUBE_ERROR_URGENT:
    fprintf(stderr, "AUBE: %s\n", msg);
    break;
  case AUBE_ERROR_FATAL:
    fprintf(stderr, "AUBE FATAL ERROR: %s\n", msg);
    break;
  default:
    break;
  }
}
