#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>

#include <hp54600b.h>

int opengnuplot(FILE** gnuplotPipe, FILE** temp){
  *temp = fopen("data.temp", "w");
  *gnuplotPipe = popen ("gnuplot -persistent", "w");

  fprintf(*gnuplotPipe, "%s \n", "set title \"HP 54600B\"");
  return 0;
}

//https://doc.ubuntu-fr.org/gnuplot
int gnuplot(char* t, int n, FILE* gnuplotPipe, FILE* temp){
  int i;

  for (i=0; i < n; i++){
    fprintf(temp, "%d %d \n", i,(int) t[i]); //Write the data to a temporary file
  }

  fprintf(gnuplotPipe, "%s \n", "plot 'data.temp' with lines");
  return 0;
}

//https://stackoverflow.com/questions/28443868/how-to-automatically-replot-a-graph-every-few-seconds-in-gnuplot
int gnureplot(char* t, int n, FILE* gnuplotPipe, FILE* temp){
  int i;

  for (i=0; i < n; i++){
    fprintf(temp, "%d %d \n", i,(int) t[i]); //Write the data to a temporary file
  }

  fprintf(gnuplotPipe, "%s \n", "replot");
  return 0;
}

//error message handling https://fr.wikibooks.org/wiki/Programmation_C/Erreurs

int main(){
  char t[100];
  int m;
  int cport_nr = RS232_OpenComport();

  FILE* temp;
  FILE* gnuplotPipe;

  if(cport_nr == -1){
    return 0;
  }

  /* wsend(cport_nr,"*RST\n"); */

  set_RST(cport_nr);

  /* wsend(cport_nr, "*CLS\n"); */

  set_CLS(cport_nr);

  /* wsend(cport_nr,"*ESE 1\n"); */

  set_ESE(cport_nr, 0);

  /* wsend(cport_nr,"*SRE 32\n"); */

  set_SRE(cport_nr, 4);

  /* bsend(cport_nr,":AUTOSCALE;*OPC?\n"); */

  set_waveform_source(cport_nr, 1);

  set_waveform_format(cport_nr);

  set_waveform_points(cport_nr,100);


  // opengnuplot(&gnuplotPipe, &temp);

  get_waveform_data(cport_nr,t);
  //    gnuplot(t,100,gnuplotPipe,temp);

  RS232_CloseComport(cport_nr);

  return(0);
}
