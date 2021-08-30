#ifndef utils_INCLUDED
#define utils_INCLUDED

int send(int, unsigned char*);
int wsend(int, unsigned char*);
int read_data(int, int, char*);
int bsend(int, unsigned char*);
int char_to_int(char);
int read_data_block(int, char**);

#endif
