#include <stdio.h>
#include <utils/utils.h>

#include "common.h"

int set_RST(int cport_nr){
  return wsend(cport_nr, "*RST\n");
}

int set_CLS(int cport_nr){
  return wsend(cport_nr, "*CLS\n");
}

int set_ESE(int cport_nr, int bit){
  int r = 0;
  switch(bit){
    // Setting bit 0 to 1 :OPC-Operation Complete
  case 0 :
    r =  wsend(cport_nr, "*ESE 1\n");
    break;
    // TODO
  default:
    printf("set_ESE: TODO :Setting bit %d not implemented\n", bit);
    break;
  }
  return r;
}

int set_SRE(int cport_nr, int bit){
  int r = 0;
  switch (bit) {
    // Setting bit 4 to 1: MAV: massage Available
  case 4 :
    r = wsend(cport_nr, "*SRE 16\n");
    break;
    // Setting bit 5 to 1: ESB: Event Status Bit
  case 5 :
    r = wsend(cport_nr, "*SRE 32\n");
    break;
    // Setting bit 6 to 1: RQS: Request Service
  case 6 :
    r = wsend(cport_nr, "*SRE 64\n");
    break;
  default:
    r = -1;
    printf("set_SRE: Bit %d not used\n", bit);
    break;
  }
  return r;
}
