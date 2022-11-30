#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <unistd.h>

#include <hp54600b.h>


int printt_data(char* t, int n){
  int i;

  for (i=0; i < n; i++){
    printf("%d %d \n", i,(int) t[i]);
  }
  return 0;
}

int main(){
  char* t;
  t = malloc(100);

  int m, size;
  int cport_nr = RS232_OpenComport("/dev/ttyUSB0", b9600);

  if(cport_nr == -1){
    return 0;
  }

  set_RST(cport_nr);
  set_CLS(cport_nr);
  set_ESE(cport_nr, 0);
  set_SRE(cport_nr, 4);

  if(autoscale(cport_nr) != 0) return -1;

  if(set_waveform_source(cport_nr, 1) != 0) return -1;
  if(set_waveform_format(cport_nr, byte) != 0) return -1;
  if(set_waveform_points(cport_nr,100) != 0) return -1;

  size = get_waveform_data(cport_nr,&t);
  if(size < 0) return -1;

  int i = 0;
  while(i < 10){
    size = get_waveform_data(cport_nr,&t);
    if(size < 0) return -1;
    printt_data(t,size);
    i++;
  }

  RS232_CloseComport(cport_nr);

  free(t);

  return(0);
}
