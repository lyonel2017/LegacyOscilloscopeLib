#ifndef utils_INCLUDED
#define utils_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <rs232/rs232.h>

int send(int, unsigned char*);
int wsend(int, unsigned char*);
int read_data(int, int, char*);
int bsend(int, unsigned char*);
int char_to_int(char);
int read_data_block(int, char*);

#endif
