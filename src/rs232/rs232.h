#ifndef rs232_INCLUDED
#define rs232_INCLUDED

#include <termios.h>

typedef enum baudrate {
       b9600 = B9600
} baudrate;

int RS232_OpenComport(char* c, baudrate b);
int RS232_PollComport(int, unsigned char *, int);
int RS232_SendByte(int, unsigned char);
int RS232_SendBuf(int, unsigned char *, int);
void RS232_CloseComport(int);
int RS232_IsDCDEnabled(int);
int RS232_IsRINGEnabled(int);
int RS232_IsCTSEnabled(int);
int RS232_IsDSREnabled(int);
int RS232_enableDTR(int);
int RS232_disableDTR(int);
int RS232_enableRTS(int);
int RS232_disableRTS(int);
void RS232_flushRX(int);
void RS232_flushTX(int);
void RS232_flushRXTX(int);

#endif
