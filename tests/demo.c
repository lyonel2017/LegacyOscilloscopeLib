#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <unistd.h>

#include <hp54600b.h>

int opengnuplot(FILE** gnuplotPipe){
  *gnuplotPipe = popen ("gnuplot", "w");

  fprintf(*gnuplotPipe, "%s \n", "set title \"HP 54600B\"");
  fflush(*gnuplotPipe);

  return 0;
}

int closegnuplot(FILE* gnuplotPipe){
  pclose(gnuplotPipe);
  return 0;
}

int gnuplot(char* t, int n, FILE* gnuplotPipe){
  int i;
  FILE* temp = fopen("data.temp", "w");

  for (i=0; i < n; i++){
    fprintf(temp, "%d %d \n", i,(int) t[i]); //Write the data to a temporary file
  }

  fclose(temp);

  fprintf(gnuplotPipe, "%s \n", "plot 'data.temp' with lines");
  fflush(gnuplotPipe);
  sleep(2);

  return 0;
}

int gnureplot(char* t, int n, FILE* gnuplotPipe){
  int i;

  FILE* temp = fopen("data.temp", "w");

  for (i=0; i < n; i++){
    fprintf(temp, "%d %d \n", i,(int) t[i]); //Write the data to a temporary file
  }

  fclose(temp);

  fprintf(gnuplotPipe, "%s \n", "replot");
  fflush(gnuplotPipe);

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

  FILE* gnuplotPipe;

  opengnuplot(&gnuplotPipe);

  set_RST(cport_nr);
  set_CLS(cport_nr);
  set_ESE(cport_nr, 0);
  set_SRE(cport_nr, 4);

  if(bsend(cport_nr,":AUTOSCALE;*OPC?\n") != 0) return -1;

  if(set_waveform_source(cport_nr, 1) != 0) return -1;
  if(set_waveform_format(cport_nr, byte) != 0) return -1;
  if(set_waveform_points(cport_nr,100) != 0) return -1;

  size = get_waveform_data(cport_nr,&t);
  if(size < 0) return -1;

  gnuplot(t,size,gnuplotPipe);

  int i = 0;
  while(i < 10){
    size = get_waveform_data(cport_nr,&t);
    if(size < 0) return -1;
    gnureplot(t,size,gnuplotPipe);
    i++;
  }

  closegnuplot(gnuplotPipe);

  RS232_CloseComport(cport_nr);

  free(t);

  return(0);
}
