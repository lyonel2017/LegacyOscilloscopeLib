# LegacyOscilloscopeLib

This project provides a library for communicating with a *HP54600B*
oscilloscope over an RS-232 interface using a *HP54658A* mesurement/storage module.
The library implement currently a small subset of the Standard Commands for Programmable Instruments (SCPI)
support by the *HP54600B* oscilloscope.
Documentation for the *HP54600B* and the supported command can be found
[here](https://www.keysight.com/us/en/product/54600B/2-channel-100-mhz-oscilloscope.html).

The mesurement module has no controler stoping the communication
when a command is processed and the command queue is full.
Thus, spaming command will mostly end in a communication failure and
a error message on the oscilliscope's screen.
It was therefore chosen that all function sending a command requires a
operation confirmation (OPC) response from the
oscilliscope before termination. This ensures that the processing of the command has terminated
on the oscilliscope side.
Using OPC requires to set some register first before any communication.
More information about OPC can be found
[here](https://www.keysight.com/us/en/lib/resources/training-materials/using-opc.html)
and in the *Programmers's Guide* manual.
Alternative solution consists in waitting for an arbitray time, until the
oscilliscope has finish processing a command, before sending the next command.

## Compilation

To compile, run make:

```
make
```

## Example

Following example show how to use the library for printing in the terminal
the data-point from channel 1.

```c
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <unistd.h>

#include <hp54600b.h>

int main(){

  // We open the serial port channel on ttyUSB0 with baude rate 9600
  // (the same baude rate must be set one the oscilloscope with hardware handshake).
  int cport_nr = RS232_OpenComport("/dev/ttyUSB0", b9600);

  if(cport_nr == -1) return -1;

  // We set register CLS so we are in a known state : all parameter on the oscilloscope
  // are set to default.
  set_RST(cport_nr);

  // We can send command to the oscilloscope, for example an autoscale, using function "send"
  // and the syntaxe given in the Programmers's Guide manual.
  if(send(cport_nr,":AUTOSCALE\n") != 0) return -1;
  // We must wait to ensure the oscilliscope has process the command
  usleep(1000000);

  // To avoid waiting:
  // we set register CLS, ESE, SRE to get operation confirmation (OPC).
  set_CLS(cport_nr);
  set_ESE(cport_nr, 0);
  set_SRE(cport_nr, 4);

  // We can now send commands to the oscilloscope using function "bsend" with "*OPC?"
  // added in sequence.
  // "bsend" is blocking until an operation confirmation is received within timeout.
  if(bsend(cport_nr,":AUTOSCALE;*OPC?\n") != 0) return -1;

  // The library provides function for sending specific command:
  // Autoscale
  if(autoscale(cport_nr) != 0) return -1;
  // Set the channel from where we want the waveform data.
  if(set_waveform_source(cport_nr, 1) != 0) return -1;
  // Set the data point precision: 8 bits (asc) or 16 bits (machine word).
  if(set_waveform_format(cport_nr, word) != 0) return -1;
  // Set the numbers of points.
  if(set_waveform_points(cport_nr,100) != 0) return -1;

  // Getting the data.
  char* t;
  t = malloc(200);
  int size;

  size = get_waveform_data(cport_nr,&t);
  if(size < 0) return -1;

  int i = 0;
  int j = 0;
  short i1;

  // Printing the data in the terminal
  while(i < size){
    i1 = (((short)t1[i]) << 8) | (0x00ff & t1[i+1]);
    printf("%d %d\n", j,(int) i1);
    i = i + 2;
    j++;
  }

  // Close the serial port channel
  RS232_CloseComport(cport_nr);

  // Free memory
  free(t);

  return(0);
}
```
More examples can be found in `/tests`.

## Intrument specification checks

The library performe checks to ensures that send commands
matches the specification of the *HP54600B* oscilloscope.
(nb. of channel, baud rate, memory size, etc.).
