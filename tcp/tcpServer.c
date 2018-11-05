#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <netdb.h>
#include <sys/stat.h>

#define TURE 1
#define FALSE 0
#define PORT 48080

int server_socket;
struct sockaddr_in address;
int addrlen;

int createsocket()
{
 int opt = 1;
 if((server_socket = socket(AF_INET,SOCK_STREAM,0)) == 0)
 {
  perror("socket failed");
  exit(EXIT_FAILURE);
 }

 if(setsockopt(server_socket,SOL_SOCKET,SO_REUSEADDR,(char *)&opt,sizeof(opt))<0)
 {
  perror("setsockopt");
  exit(EXIT_FAILURE);
 }

 address.sin_family = AF_INET;
 address.sin_addr.s_addr = INADDR_ANY;
 address.sin_port = htons(PORT);

 if(bind(server_socket,(struct sockaddr *)&address,sizeof(address))<0)
 {
  perror("bind failed");
  exit(EXIT_FAILURE);
 }
 printf("Linstener on port %d \n",PORT);

 if(listen(server_socket,3)<0)
 {
  perror("llisten");
  exit(EXIT_FAILURE);
 }
 
 addrlen = sizeof(address);
 return 1;
}

int main()
{
 int new_socket;
 char recv_buffer[1024];
 int recvck = 0;
 int Socket_OK = 0;

 Socket_OK = createsocket();

 if(Socket_OK == 1)
 {
  printf("Create Socket OK Watting for connect ...\n");
  printf("Port is %d \n",PORT);
 }
 
 while(Socket_OK)
 {
  if((new_socket = accept(server_socket,(struct sockaddr*)&address,(socklen_t*)&addrlen))<0)
  {
   perror("accept");
   exit(EXIT_FAILURE);
  }
  recvck = recv(new_socket,recv_buffer,sizeof(recv_buffer),0);
  recv_buffer[recvck] = '\0';
  printf("recv <= %s\n",recv_buffer);
  break;
 }
 close(server_socket);
 close(new_socket);
 return 0;
}
