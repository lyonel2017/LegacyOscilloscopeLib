#ifndef waveform_INCLUDED
#define waveform_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <utils/utils.h>

int get_waveform_data(int, char*);
int set_waveform_source(int, int);
int set_waveform_format(int);
int set_waveform_points(int,int);

#endif
