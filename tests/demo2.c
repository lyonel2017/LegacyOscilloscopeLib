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
  return pclose(gnuplotPipe);
}

int gnuplot(char* t1, int n, FILE* gnuplotPipe){
  FILE* temp = fopen("data.temp", "w");

  int i,j;
  short i1;

  i = 0;
  j = 0;

  while(i < n){
    i1 = (((short)t1[i]) << 8) | (0x00ff & t1[i+1]);
    fprintf(temp, "%d %d\n", j,(int) i1);
    i = i + 2;
    j++;
  }

  fclose(temp);

  fprintf(gnuplotPipe, "%s \n", "plot 'data.temp' using 1:2 with lines");

  /* fprintf(gnuplotPipe, "%s \n", "plot 'data.temp' using 1:2 with lines,\ */
  /*                                     'data.temp' using 1:3 with lines"); */

  fflush(gnuplotPipe);
  sleep(2);

  return 0;
}

int gnureplot(char* t1, int n, FILE* gnuplotPipe){
  FILE* temp = fopen("data.temp", "w");

  int i,j;
  short i1;

  i = 0;
  j = 0;

  while(i < n){
    i1 = (((short)t1[i]) << 8) | (0x00ff & t1[i+1]);
    fprintf(temp, "%d %d\n", j,(int) i1);
    i = i + 2;
    j++;
  }

  fclose(temp);

  fprintf(gnuplotPipe, "%s \n", "replot");
  fflush(gnuplotPipe);

  return 0;
}

int main(){
  char* t1;
  t1 = malloc(10);

  int m, size;

  int cport_nr = RS232_OpenComport("/dev/ttyUSB4", b9600);

  if(cport_nr == -1){
    return 0;
  }

  FILE* gnuplotPipe;

  opengnuplot(&gnuplotPipe);

  set_RST(cport_nr);
  set_CLS(cport_nr);
  set_ESE(cport_nr, 0);
  set_SRE(cport_nr, 4);

  char str1;

  printf("Press any key to start\n");
  str1 = getchar();
  if (str1 != 'q'){

    set_waveform_source(cport_nr, 1);
    set_waveform_format(cport_nr, word);
    set_waveform_points(cport_nr,2000);

    size = get_waveform_data(cport_nr,&t1);
    if(size < 0) return -1;

    printf("test %d\n",size);

    gnuplot(t1,size,gnuplotPipe);

    printf("Press Enter to start\n");
    str1 = getchar();

    while(str1 != 'q'){
      set_waveform_source(cport_nr, 1);
      set_waveform_format(cport_nr, word);
      set_waveform_points(cport_nr,2000);

      size = get_waveform_data(cport_nr,&t1);
      if(size < 0) return -1;

      gnureplot(t1,size,gnuplotPipe);

      printf("Press Enter to restart\n");
      str1 = getchar();
    }
  }

  RS232_CloseComport(cport_nr);

  free(t1);

  closegnuplot(gnuplotPipe);

  return(0);
}
