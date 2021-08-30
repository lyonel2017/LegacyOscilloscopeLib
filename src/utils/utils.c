#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include <rs232/rs232.h>

#include "utils.h"

#define sleep 1000000

int send(int cport_nr, unsigned char* buff){
  int n = 0;
  n = RS232_SendBuf(cport_nr, buff,strlen(buff));
  if(n < 0){
    printf("send:RS232_SendBuf: Fail sending string %s\n", buff);
    return -1;
  }
  printf("send %d bytes: %s", n, buff);
  return 0;
}

int wsend(int cport_nr, unsigned char* buff){
  int n = 0;
  n = RS232_SendBuf(cport_nr, buff,strlen(buff));
  if(n < 0){
    printf("wsend:RS232_SendBuf: Fail sending string %s\n", buff);
    return -1;
  }
  usleep(sleep);
  printf("wsend %d bytes: %s", n, buff);
  return 0;
}

int read_data (int cport_nr, int count, char* buff2){
  int m = 0;
  int n;
  int try = 0;
  char buff1[count];
  while(m < count){
    n = RS232_PollComport(cport_nr, buff1, count-m);
    if(n > 0){
      memcpy(buff2 + m, buff1,n);
      m = m + n;
      try = 0;
    }
    if(n == 0 && try > 3){
      printf("read_data: No data to read at level %d of %d\n",m,count);
      return -1;
    }
    if(n < 0){
      printf("read_data: Fail to read data at level %d of %d\n",m,count);
      return -1;
    }
    if (n == 0 && try <= 3){
      try++;
    }
  }
  return 0;
}

int bsend(int cport_nr, unsigned char* buff){
  int n = 0;
  int m = 0;
  char buff2[3];

  n = RS232_SendBuf(cport_nr, buff,strlen(buff));
  if(n < 0){
    printf("bsend:RS232_SendBuf: Fail sending string %s\n", buff);
    return -1;
  }

  if(read_data(cport_nr,3,buff2) == -1){
    printf("bsend:read_data: Fail receiving confirmation of "
           "work completion:: Fail sending string %s\n", buff );
    return -1;
  }

  //chech the 3 caracter

  if(!(buff2[0] == '+' && buff2[1] == '1')){
    printf("bsend: Receive non conform confirmation of work "
           "completion:: Fail sending string %s\n", buff );
    return -1;
  }

  printf("bsend %d bytes: %s\n", n, buff);

  return 0;
}

int char_to_int(char c){
  if (c >= 48 && c <= 57){
    return c - 48;
  }
  else {
    printf("char_to_int: Unable to convert character %c to integer\n",c);
    return -1;
  }
}

int read_data_block(int cport_nr, char** t){
  char buff1[2];
  int m;

  if(read_data(cport_nr,2,buff1) == -1){
    printf("read_block_data:read_data: Fail reading starting "
            "character for received data block\n");
    return -1;
  }

  if(buff1[0] != '#'){
    printf("read_block_data: Unexpected starting character for "
           "received data block: %c\n", buff1[0]);
    return -1;
  }
  int num_size = char_to_int(buff1[1]);

  if(num_size == -1){
    printf("read_block_data:char_to_int: Block size is bad. Get %c\n",buff1[1]);
    return -1;
  }

  char buff2[num_size];

  if(read_data(cport_nr,num_size,buff2) == -1){
    printf("read_block_data:read_data: Fail reading %d bytes of data block size\n",
            num_size);
    return -1;
  }

  int size = 0, tmp = 0;

  m = 0;
  while(m < num_size){
    tmp = char_to_int(buff2[m]);
    if(tmp == -1){
      printf("read_block_data:char_to_int: Element %d of block is bad. Get %c\n",
             m,buff2[m]);
      return -1;
    }
    size = size * 10 + tmp;
    m++;
  }

  *t = realloc(*t,size); //use specific data structure to avoid calling always realloc

  if (*t == NULL){
    fprintf(stderr, "read_block_data:realloc: Fail realloc array for size %d: ",
            size);
    perror("");
    return -1;
  }

  if(read_data(cport_nr,size,*t) == -1){
    printf("read_block_data:read_data: Fail reading %d bytes of data block\n",size);
    return -1;
  }

  if(read_data(cport_nr,1,buff1)){
    printf("read_block_data:read_data: Fail reading ending character"
            "for received data block\n");
    return -1;
  }

  if(buff1[0] != '\n'){
    printf("read_block_data: Unexpected ending character for received"
            "data block: %c\n", buff1[0]);
    return -1;
  }

  return size;
}
