#ifndef hp54600b_INCLUDED
#define hp54600b_INCLUDED

#include <termios.h>

/* RS232 */
typedef enum baudrate {
       b9600 = B9600
} baudrate;

int RS232_OpenComport(char* c, baudrate b);
void RS232_CloseComport(int);

/* Utils for communication */
int send(int, unsigned char*);
int wsend(int, unsigned char*);
int read_data(int, int, char*);
int bsend(int, unsigned char*);
int char_to_int(char);
int rea_data_block(int, char*);

/* IEEE 488.2 Common command */
int set_RST(int cport_nr);
int set_CLS(int cport_nr);
int set_ESE(int cport_nr, int bit);
int set_SRE(int cport_nr, int bit);

/* Waveform command */
int get_waveform_data(int, char*);
int set_waveform_source(int, int);
int set_waveform_format(int);
int set_waveform_points(int,int);

#endif
