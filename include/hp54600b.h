#ifndef hp54600b_INCLUDED
#define hp54600b_INCLUDED

/* RS232 */
int RS232_OpenComport();
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

/* Utils for communication*/
int send(int, unsigned char*);
int wsend(int, unsigned char*);
int read_data(int, int, char*);
int bsend(int, unsigned char*);
int char_to_int(char);
int read_data_block(int, char*);

/* Waveform*/
int get_waveform_data(int, char*);
int set_waveform_source(int, int);
int set_waveform_format(int);
int set_waveform_points(int,int);

#endif
