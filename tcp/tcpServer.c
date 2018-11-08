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
#include <json/json.h>

#define TURE 1
#define FALSE 0
#define PORT 48080

int server_socket;
struct sockaddr_in serverAddress;
int addrlen;


int client_socket;
struct sockaddr_in clientAddress;

json_object *interface;

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

 serverAddress.sin_family = AF_INET;
 serverAddress.sin_addr.s_addr = INADDR_ANY;
 serverAddress.sin_port = htons(PORT);

 if(bind(server_socket,(struct sockaddr *)&serverAddress,sizeof(serverAddress))<0)
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
 
 addrlen = sizeof(serverAddress);



 return 1;
}


int main()
{
 int new_socket;
 char recv_buffer[1024];
 char sendbuf[1024];
 int recvck = 0;
 int Socket_OK = 0;
 int client_Socket_OK = 0;

 Socket_OK = createsocket();

 if(Socket_OK == 1)
 {
  printf("Create Socket OK Watting for connect ...\n");
  printf("Port is %d \n",PORT);
 }
 

 while(Socket_OK)
 {
  if((new_socket = accept(server_socket,(struct sockaddr*)&serverAddress,(socklen_t*)&addrlen))<0)
  {
   perror("accept");
   exit(EXIT_FAILURE);
  }
 
  recvck = recv(new_socket,recv_buffer,sizeof(recv_buffer),0);
  recv_buffer[recvck] = '\0';
  printf("recv <= %s\n",recv_buffer);
 
	 if(strstr(recv_buffer,"addDevice") != NULL ) {
	  strcpy(sendbuf,"add device");
	  printf("send =>%s\n",sendbuf);
	  send(new_socket,sendbuf,sizeof(sendbuf),0);
	  close(new_socket);
	 } else if(strstr(recv_buffer,"removeDevice") != NULL ) {
		interface = json_object_new_object();
		json_object_object_add(interface, "MessageType", json_object_new_string("Node Remove Status"));
		json_object_object_add(interface, "Status", json_object_new_string("Failed")); 
		strcpy(sendbuf,json_object_to_json_string(interface));
		printf("send =>%s\n",sendbuf);
		send(new_socket,sendbuf,sizeof(sendbuf),0);
		close(new_socket);
	 } 

 
  //break;
 }

 close(server_socket);
 close(new_socket);
 return 0;
}

