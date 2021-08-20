#ifndef waveform_INCLUDED
#define waveform_INCLUDED

typedef enum format { asc, word, byte } format;

int get_waveform_data(int, char*);
int set_waveform_source(int, int);
int set_waveform_format(int, format f);
int set_waveform_points(int,int);

#endif
