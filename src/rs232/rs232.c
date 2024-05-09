/*
  Following interface is inspired by:
  https://gitlab.com/Teuniz/RS-232/-/blob/master/rs232.c
*/

/*https://gitlab.com/Teuniz/RS-232/-/blob/master/rs232.c*/

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include <sys/types.h>

#include <sys/stat.h>
#include <limits.h>
#include <sys/file.h>

#include "rs232.h"

int RS232_OpenComport(char* c, baudrate b)
{
  int fd;
  struct termios new_port_settings;

  fd = open(c, O_RDWR | O_NOCTTY | O_NDELAY);

  if(fd == -1)
    {
      fprintf(stderr, "RS232_OpenComport:open: Unable to open %s: ", c);
      perror("");
      return -1;
    }

  /*
   * Lock access so that another process can't use the port
   */
  if(flock(fd, LOCK_EX | LOCK_NB) != 0)
    {
      close(fd);
      fprintf(stderr, "RS232_OpenComport:flock: Another process has locked %s: ", c);
      perror("");
      return -1;
    }

  /*
   * Reading Data from the Port with normal (blocking) behavior,
   */
  if(fcntl(fd, F_SETFL, 0) == -1)
    {
      close(fd);
      flock(fd, LOCK_UN);  /* free the port so that others can use it. */
      fprintf(stderr, "RS232_OpenComport:fcntl: Unable to set F_SETFL to 0 for %s: ", c);
      perror("");
      return -1;
    }

  /*
   * Init port settings
   */
  bzero(&new_port_settings, sizeof(new_port_settings));

  /*
    BAUDRATE: Set bps rate. You could also use cfsetispeed and cfsetospeed.
    CS8     : 8n1 (8bit,no parity,1 stopbit)
    CLOCAL  : local connection, no modem contol
    CREAD   : enable receiving characters
  */
  new_port_settings.c_cflag = b | CS8 | CLOCAL | CREAD;

  /*
   * Choosing Raw Input
   */
  new_port_settings.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

  /*
    °   * Choosing Raw Output
  */
  new_port_settings.c_oflag = ~OPOST;

  /*
   * Setting Hardware Flow Control
   */

  new_port_settings.c_cflag |= CRTSCTS;

  /*
   * Setting Read Timeouts
   */
  new_port_settings.c_cc[VMIN] = 0;  /* block untill 0 bytes are received */
  new_port_settings.c_cc[VTIME] = 10;/* block untill 1 Sec expires */

  /*
   * Set the new options for the port
   */
  if(tcsetattr(fd, TCSANOW, &new_port_settings) == -1)
    {
      close(fd);
      flock(fd, LOCK_UN);  /* free the port so that others can use it. */
      fprintf(stderr,
	      "RS232_OpenComport:tcsetattr: Unable to set portsettings of %s: ", c);
      perror("");
      return -1;
    }

  /*
   * Set RST and DTR pin to 1
   */
  if(RS232_enableRTS(fd)==-1 ||  RS232_enableDTR(fd) == -1)
    {
      flock(fd, LOCK_UN);  /* free the port so that others can use it. */
      fprintf(stderr, "RS232_OpenComport:RS232_enableRTS/RS232_enableDTR: "
	      "Unable to set pin DTR and RTS to 1 for %s: ", c);
      perror("");
      return -1;
    }

  /*
   * Flush send and recv buffer
   */
  RS232_flushRXTX(fd);

  return fd;
}

void RS232_CloseComport(int fd)
{
  close(fd);
  flock(fd, LOCK_UN);  /* free the port so that others can use it. */
  return;
}

int RS232_PollComport(int fd, char* buf, int size)
{
  int n;
  n = read(fd, buf, size);
  if(n < 0)
    {
      perror("RS232_PollComport:read");
      return -1;
    }
  return n;
}

int RS232_SendByte(int fd, const char byte)
{
  int n = write(fd, &byte, 1);
  if(n < 0)
    {
      perror("RS232_SendByte:write");
      return -1;
    }
  return 0;
}

int RS232_SendBuf(int fd, const char* buf, int size)
{
  int n = write(fd, buf, size);
  if(n < 0)
    {
      perror("RS232_SendBuf:write");
      return -1;
    }
  return n;
}

int RS232_IsDCDEnabled(int fd)
{
  int status;

  if(ioctl(fd, TIOCMGET, &status) == -1)
    {
      perror("RS232_IsDCDEnabled:ioctl: unable to get port status");
      return -1;
    }

  if(status&TIOCM_CAR) return 1;

  return 0;
}

int RS232_IsRINGEnabled(int fd)
{
  int status;

  if(ioctl(fd, TIOCMGET, &status) == -1)
    {
      perror("RS232_IsRINGEnabled:ioctl: unable to get port status");
      return -1;
    }

  if(status&TIOCM_RNG) return 1;

  return 0;
}

int RS232_IsCTSEnabled(int fd)
{
  int status;

  if(ioctl(fd, TIOCMGET, &status) == -1)
    {
      perror("RS232_IsCTSEnabled:ioctl: unable to get port status");
      return -1;
    }

  if(status&TIOCM_CTS) return 1;

  return 0;
}

int RS232_IsDSREnabled(int fd)
{
  int status;

  if(ioctl(fd, TIOCMGET, &status) == -1)
    {
      perror("RS232_IsDSREnabled:ioctl: unable to get port status");
      return -1;
    }

  if(status&TIOCM_DSR) return 1;

  return 0;
}

int RS232_enableDTR(int fd)
{
  int status;

  if(ioctl(fd, TIOCMGET, &status) == -1)
    {
      perror("RS232_enableDTR:ioctl: unable to get ports tatus");
      return -1;
    }

  status |= TIOCM_DTR;    /* turn on DTR */

  if(ioctl(fd, TIOCMSET, &status) == -1)
    {
      perror("RS232_enableDTR:ioctl: unable to set portstatus");
      return -1;
    }
  return 0;
}

int RS232_disableDTR(int fd)
{
  int status;

  if(ioctl(fd, TIOCMGET, &status) == -1)
    {
      perror("RS232_disableDTR:ioctl: unable to get portstatus");
      return -1;
    }

  status &= ~TIOCM_DTR;    /* turn off DTR */

  if(ioctl(fd, TIOCMSET, &status) == -1)
    {
      perror("RS232_disableDTR:ioctl: unable to set portstatus");
      return -1;
    }
  return 0;
}

int RS232_enableRTS(int fd)
{
  int status;

  if(ioctl(fd, TIOCMGET, &status) == -1)
    {
      perror("RS232_enableRTS:ioctl: unable to get portstatus");
      return -1;
    }

  status |= TIOCM_RTS;    /* turn on RTS */

  if(ioctl(fd, TIOCMSET, &status) == -1)
    {
      perror("RS232_enableRTS:ioctl: unable to set portstatus");
      return -1;
    }
  return 0;
}

int RS232_disableRTS(int fd)
{
  int status;

  if(ioctl(fd, TIOCMGET, &status) == -1)
    {
      perror("RS232_disableRTS:ioctl: unable to get portstatus");
      return -1;
    }

  status &= ~TIOCM_RTS;    /* turn off RTS */

  if(ioctl(fd, TIOCMSET, &status) == -1)
    {
      perror("RS232_disableRTS:ioctl: unable to set portstatus");
      return -1;
    }
  return 0;
}

void RS232_flushRX(int fd)
{
  tcflush(fd, TCIFLUSH);
}


void RS232_flushTX(int fd)
{
  tcflush(fd, TCOFLUSH);
}


void RS232_flushRXTX(int fd)
{
  tcflush(fd, TCIOFLUSH);
}
