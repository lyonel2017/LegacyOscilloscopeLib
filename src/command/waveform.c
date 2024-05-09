#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <utils/utils.h>

#include "waveform.h"

int get_waveform_data(int cport_nr, char **t)
{
  if(send(cport_nr, ":WAVEFORM:DATA?\n") != 0) return -1;

  return read_data_block(cport_nr, t);
}

int set_waveform_source(int cport_nr, int c)
{
  switch(c)
    {
    case 1 :
      return bsend(cport_nr,":WAVEFORM:SOURCE CHANNEL1;*OPC?\n");
    case 2 :
      return bsend(cport_nr,":WAVEFORM:SOURCE CHANNEL2;*OPC?\n");
    default :
      printf("waveform_source: No source CHANNEL%d "
             "(support source : CHANNEL1 and CHANNEL2)\n", c);
      return -1;
    }
}

int set_waveform_format(int cport_nr, format f)
{
  switch(f)
    {
    case byte :
      return bsend(cport_nr, ":WAVEFORM:FORMAT BYTE;*OPC?\n");
    case word :
      return bsend(cport_nr, ":WAVEFORM:FORMAT WORD;*OPC?\n");
    case asc :
      return bsend(cport_nr, ":WAVEFORM:FORMAT ASC;*OPC?\n");
    default :
      return -1;
    }
}

int set_waveform_points(int cport_nr, int p)
{
  switch(p)
    {
    case 100 :
      return bsend(cport_nr, ":WAVEFORM:POINTS 100;*OPC?\n");
    case 200 :
      return bsend(cport_nr, ":WAVEFORM:POINTS 200;*OPC?\n");
    case 250 :
      return bsend(cport_nr, ":WAVEFORM:POINTS 250;*OPC?\n");
    case 400 :
      return bsend(cport_nr, ":WAVEFORM:POINTS 400;*OPC?\n");
    case 500 :
      return bsend(cport_nr, ":WAVEFORM:POINTS 500;*OPC?\n");
    case 800 :
      return bsend(cport_nr, ":WAVEFORM:POINTS 800;*OPC?\n");
    case 1000 :
      return bsend(cport_nr, ":WAVEFORM:POINTS 1000;*OPC?\n");
    case 2000 :
      return bsend(cport_nr, ":WAVEFORM:POINTS 2000;*OPC?\n");
    case 4000 :
      return bsend(cport_nr, ":WAVEFORM:POINTS 4000;*OPC?\n");
    case 5000 :
      return bsend(cport_nr, ":WAVEFORM:POINTS 5000;*OPC?\n");
    default :
      printf("waveform_point: Unsupported point numbers %d\n", p);
      return -1;
    }
}
