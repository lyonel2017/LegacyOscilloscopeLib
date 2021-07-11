# LegacyOscilloscopeLib

This project provides a library for communicating with a *HP54600B*
oscilliscope over RS-232 interface using a *HP54658A* mesurement/storage module.
The library implement currently a small subset of the command support by
the *HP56100B*.
Documentation about the oscilliscope and the supported command can be found
[here](https://www.keysight.com/us/en/product/54600B/2-channel-100-mhz-oscilloscope.html)
or in `/docs`.

The mesurement module has no controler stoping the communication
when the command queue is full and a command is processed.
Thus, spaming command will mostly end in a communication failure and
a error message on the oscilliscope's screen.
It was therefore chosen that all function sending a command requires a
operation confirmation (OPC) response from the
oscilliscope before termination, to ensures the processing of the command has terminated.
This requires to set the aproprieted register first before any communication.
More information about OPC can be found
[here](https://www.keysight.com/us/en/lib/resources/training-materials/using-opc.html)
and in the *Programmers's Guide* manual.
An alternative solution is waitting for an arbitray time, hopping the
scope has finish processing, before sending the next command.

## Compilation

TODO

## Example

Examples can be found in `/tests`.


<!-- The library also provide different option for communicating with the -->
<!-- scope, allowing to spam without error. -->

<!-- Serial port is slow so the refresh frequence is slow. -->
<!-- Using maximum bodraite -->
