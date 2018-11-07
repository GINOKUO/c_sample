#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#define PORT 48080


int clientsocket;
struct sockaddr_in serveraddr;

int createsocket()
{
 if((clientsocket = socket(AF_INET,SOCK_STREAM,0)) < 0)
 {
  perror("socket error");
  return -1;
 }
 else  printf("socket OK\n");
 
 serveraddr.sin_family = AF_INET;
 serveraddr.sin_port = htons(PORT);
 serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");

 if(connect(clientsocket,(struct sockaddr *)&serveraddr,sizeof(serveraddr)) < 0)
 {
  perror("connect");
  exit(1);
 }
 else
 {
  printf("connect to server\n");
  return 1;
 }
}

int main()
{
 char sendbuf[1024];
 int Socket_OK = 0;
 
 Socket_OK = createsocket();
 
 if(Socket_OK == 1)
 {
  printf("OK\n");
 }

 while(Socket_OK)
 {
  strcpy(sendbuf,"removeDevice");
  printf("send =>%s\n",sendbuf);
  send(clientsocket,sendbuf,sizeof(sendbuf),0);
  close(clientsocket);
  break;
 }
 
 return 0;
}
