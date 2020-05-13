/* stack.c */

/* This program has a buffer overflow vulnerability. */
/* Our task is to exploit this vulnerability, not by
 * modifying this code, but by providing a cleverly
 * constructed input. */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <unistd.h>

ssize_t bof(int sockfd)
{
  char buffer[32];
  char resp[32];
  char str[1001];
  struct sockaddr_in addr;
  struct sockaddr* saddr = (struct sockaddr*) &addr;
  socklen_t addr_size = sizeof(addr);
  ssize_t s;

  bzero(str,1001);
  bzero(buffer,32);
  s = recvfrom(sockfd, str, 1000, 0, saddr, &addr_size);
  printf("server received %d bytes\n",s);
  memcpy(buffer,str,s);
  sprintf(resp,str);
  s = sendto(sockfd, resp, strlen(resp), 0, saddr, addr_size);
  if ( s < 0 ) {
    printf("send failed with error %d\n", errno);
  }
  return s;
}


int main(int argc, char **argv)
{
  int s = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
  struct sockaddr_in addr;
  bzero(&addr, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = htonl(INADDR_ANY);
  addr.sin_port = htons(8000);
  bind(s, (struct sockaddr*)&addr, sizeof(addr));

  while(1) {
    ssize_t r = bof(s);
    if ( r < 0 ) {
      break;
    }
    printf("sent %d bytes\n", r);
  }

  printf("Returned Properly\n");
  return 1;
}
