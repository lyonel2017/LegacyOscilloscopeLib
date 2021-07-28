#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <utils/utils.h>

#include "waveform.h"

int get_waveform_data(int cport_nr, char* t){

  if(send(cport_nr, ":WAVEFORM:DATA?\n") != 0) return -1;

  if(read_data_block(cport_nr, t) != 0) return -1;

  return 0;
}

int set_waveform_source(int cport_nr, int c){
  switch(c){
  case 1 :
    return bsend(cport_nr,":WAVEFORM:SOURCE CHANNEL1;*OPC?\n");
  case 2 :
    return bsend(cport_nr,":WAVEFORM:SOURCE CHANNEL2;*OPC?\n");
  default :
    printf("waveform_source: No source CHANNEL%d (support source : CHANNEL1 and CHANNEL2)\n", c);
    return -1;
  }
}

int set_waveform_format(int cport_nr){
  return bsend(cport_nr, ":WAVEFORM:FORMAT BYTE;*OPC?\n");
}

int set_waveform_points(int cport_nr,int p){
  switch(p){
  case 100 :
    return bsend(cport_nr, ":WAVEFORM:POINTS 100;*OPC?\n");
  default :
    printf("waveform_point: No point numbers %d (support point number : 100)\n", p);
    return -1;
  }
}
