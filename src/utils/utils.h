#ifndef utils_INCLUDED
#define utils_INCLUDED

int send(int, const char*);
int wsend(int, const char*);
int read_data(int, int, char*);
int bsend(int, const char*);
int char_to_int(char);
int read_data_block(int, char**);

#endif
