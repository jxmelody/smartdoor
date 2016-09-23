#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <sqlite3.h>
#include <pthread.h>
#include "cJSON.h"
#include "motorioctl.h"
#include "cccjson.h"
#include "door.h"
#include "log.h"
#include "login.h"
#include "register.h"

#define PORT 30000
#define MAX_BUFFER 1024
#define  BACKLOG  2
#define CLIENTNUM 2

#define REG_SUCCESS 01
#define REG_FAILED 02
#define LOG_SUCCESS 03
#define LOG_FAILED 04
#define SIGN_SUCCESS 05
#define SIGN_FAILED 06
#define OPEN_SUCCESS 07
#define OPEN_FAILED 08
#define CLOSE_SUCCESS 09
#define CLOSE_FAILED 10
#define VIEW_LOG_SUCCESS 11
#define VIEW_LOG_FAILED 12

pthread_mutex_t ALOCK = PTHREAD_MUTEX_INITIALIZER;
struct jsonvalue{
	char sign[10];
	char username[20];
	char password[20];
	char regcode[20];
	};

	static void *handle_request( void *argv)
		  {
		  	int client_sockfd;
		  	int server_sockfd;
		  	 struct sockaddr_in client_addr;
			socklen_t addrlen = sizeof(struct sockaddr);
			 /* accept a connection */
			printf("waiting connection...\n");
				
			for(;;)
			{
				int size = 0;
				char *opera;
				char buffer[MAX_BUFFER + 1];
				char cbuffer[MAX_BUFFER + 1];
				char *buff,*b;
				pthread_mutex_lock(&ALOCK);
				client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_addr, &addrlen);
				pthread_mutex_unlock(&ALOCK);
				memset(cbuffer,0, MAX_BUFFER+1);
				memset(buffer,0, MAX_BUFFER+1);
				size = recv(client_sockfd, buffer, MAX_BUFFER+1,0);
				//buffer[size]='\0';
				if(size>0)
				{
					printf("%s\n",buffer);
				}
				/*  *** process ****/
				struct jsonvalue recive_value;
				recive_value=parseJson(buffer); 
				if (strcmp(recive_value.sign,"login")==0)
				{
					printf("login!!!!!");
					opera="login";
					cbuffer[1]= login(recive_value.username,recive_value.password);
					lupdate(recive_value.username,opera);
					buff=makeJson_sign(cbuffer[1]);
					sprintf(buffer,"%24s\r\n",buff);
				}
				else if (strcmp(recive_value.sign,"reg")==0)
				{
					printf("register!!!!!");
					opera="register";
					cbuffer[1]= regist(recive_value.username,recive_value.password,recive_value.regcode);
					lupdate(recive_value.username,opera);
					buff=makeJson_sign(cbuffer[1]);
					sprintf(buffer,"%24s\r\n",buff);
				}
				else if(strcmp(recive_value.sign,"opendoor")==0)
				{
					opera="opendoor";
					cbuffer[1]= open_door();
					lupdate(recive_value.username,opera);
					buff=makeJson_sign(cbuffer[1]);
					sprintf(buffer,"%24s\r\n",buff);
				}
				else if(strcmp(recive_value.sign,"closedoor")==0)
				{
					opera="closedoor";
					cbuffer[1]= close_door();
					lupdate(recive_value.username,opera);
					buff=makeJson_sign(cbuffer[1]);
					sprintf(buffer,"%24s\r\n",buff);
				}
				else if(strcmp(recive_value.sign,"viewlog")==0)
				{
					b= viewlog();
					buff=makeJson_logfi(b);
					sprintf(buffer,"%24s\r\n",buff);
				}
				else if(strcmp(recive_value.sign,"creatcode")==0)
				{
					opera="create";
					b= createcode();
					lupdate(recive_value.username,opera); 
					buff=makeJson_regcode(b);
					sprintf(buffer,"%24s\r\n",buff);
				}
				/*  *** process ****/
				send(client_sockfd, buffer, strlen(buffer)+1,0);
				//printf("connection established!\n");	
				close(client_sockfd);
			}
			return NULL;
		}
		

static void handle_connect(int s)
	{
		printf("waiting message...\n");
		
		int server_sockfd = s;
		int i = 0;
		pthread_t thread_do[CLIENTNUM];
		for(i=0;i<CLIENTNUM;i++)
		{		
			pthread_create(&thread_do[i],NULL,handle_request,(void*)&server_sockfd);
		}
		for(i=0;i<CLIENTNUM;i++)
		{		
			pthread_join(thread_do[i],NULL);
		}
	}
	
	

int main()
{
	  /* create a socket */
	  int server_sockfd ;
	  struct sockaddr_in server_addr;
	 
	  pid_t pid;
	  int err;
	  
	  
	  server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	  if (server_sockfd<0)
	  {
	  	printf("socket error\n");
	  	return -1;
	  }
	    
	   bzero(&server_addr,sizeof(server_addr));
	  server_addr.sin_family = AF_INET;//设置协议簇
	 server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");//绑定本机的网卡
	   //server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	  server_addr.sin_port = htons(PORT);//绑定端口号
	  
	  /* bind with the local file */
	  int yes=1;
	//char yes='1'; // use this under Solaris

	if (setsockopt(server_sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
	    perror("setsockopt");
	    exit(1);
	}
	 err= bind(server_sockfd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr));
	 if(err<0)
	 {
	 	printf("bind error\n");
	 	return -1;
	 }
	 
	  /* listen */
	 if (-1 == listen(server_sockfd, BACKLOG)) 
	{
		  perror("listen error.");
		  return -1;
	 }
	  handle_connect(server_sockfd);
	  close(server_sockfd);
	  
	  return 0;
}
