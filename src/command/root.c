#include <utils/utils.h>

#include "root.h"

int autoscale(int cport_nr)
{
  return bsend(cport_nr, ":AUTOSCALE;*OPC?\n");
}
