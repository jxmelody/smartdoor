#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include "sqlite3.h"
#include "cJSON.h"
#include "motorioctl.h"
#include "cccjson.h"
#include "door.h"
#include "log.h"
#include "login.h"
#include "register.h"
#define BACKLOG 5
#define REG_SUCCESS 1
#define REG_FAILED 2
#define LOG_SUCCESS 1
#define LOG_FAILED 2
#define SIGN_SUCCESS 1
#define SIGN_FAILED 2
#define LOG_OUT 4
#define OPEN_SUCCESS 5
#define OPEN_FAILED 6
#define CLOSE_SUCCESS 7
#define CLOSE_FAILED 8
#define VIEW_LOG_SUCCESS 1
#define VIEW_LOG_FAILED 2
#define PORT 30000
#define MAX_BUFFER 2048


char *sign,*username,*password, *regcode;
int parseJson(char * pMsg)
{	
	if(NULL == pMsg)
	{
		
	}
	cJSON * pJson = cJSON_Parse(pMsg);
	if(NULL == pJson) 
	{
		
	}

	// get sign from json
	cJSON * pSub1 = cJSON_GetObjectItem(pJson, "sign");
	if(NULL == pSub1)
	{
		//get object named "hello" faild
	}
	else{
		sign=pSub1->valuestring;
		printf("sign : %s\n",sign);
	}

	// get username from json
	cJSON * pSub2 = cJSON_GetObjectItem(pJson, "username");
	if(NULL == pSub2)
	{
		//get object named "hello" faild
	}
	else{
		username=pSub2->valuestring;
		printf("username : %s\n", username);
	}

	// get password from json
	cJSON *pSub3 = cJSON_GetObjectItem(pJson, "password");
	if(NULL == pSub3)
	{
		//get password from json faild
	}
	else{
		password=pSub3->valuestring;
		printf("password : %s\n",password);
	}
	
	// get regcode from json
	cJSON *pSub4 = cJSON_GetObjectItem(pJson, "reg_code");
	if(NULL == pSub4)
	{
		//get regcode from json faild
	}
	else{
		regcode=pSub4->valuestring;
		printf("regcode : %s\n",regcode);
	}
	//cJSON_Delete(pJson);
	return 0;
}
	
int main()
{
	system("insmod /motor.ko");
	  /* create a socket */
	  int server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	  
	  struct sockaddr_in server_addr;
	  server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr("192.168.2.1");
	//  server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	  server_addr.sin_port = htons(PORT);
	  
	  /* bind with the local file */
	  bind(server_sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
	  
	  /* listen */
	  listen(server_sockfd, BACKLOG);
	  
	  int size;
	  char buffer[MAX_BUFFER + 1];
	  int client_sockfd;
	  struct sockaddr_in client_addr;
	  socklen_t len = sizeof(client_addr);
	  
	  /* accept a connection */
	printf("waiting connection...\n");
	
	  	
	  while(1)
	  {
	  		client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_addr, &len);
			printf("connection established!\n");
			memset(buffer,0, MAX_BUFFER+1);
	  		size = read(client_sockfd, buffer, MAX_BUFFER+1);
	  		if (size==0)  continue;
	  		printf("%d\n",size);
			buffer[size] = '\0';
			int ch=0 ;
			int cv=2;
			int cc=2;
			char *opera;
			char *buff,*bc,*bv;
			buff = buffer;
			if(buff!=0)
			{	
				printf("%s\n",buff);
			}
			 
			parseJson(buff); 
				if (strcmp(sign,"reg")==0)
					{	
						
						opera="register";
						ch= regist( username,password, regcode);
						if(ch==REG_SUCCESS)
							{
								lupdate( username,opera);
							}
						buff=makeJson_sign(ch);
						sprintf(buffer,"%s\r\n",buff);
						write(client_sockfd,buffer,MAX_BUFFER);
					}

				if (strcmp(sign,"login")==0)
					{
						int loginflag=1;
						ch=0;
						opera="login";
						ch=login(username,password);
						if (size==0)  continue;
						printf("ch:%d\n",ch);
						if(ch==LOG_SUCCESS)
						{
							lupdate(username,opera);
							loginflag=0;
						}
						buff=makeJson_sign(ch); 
						sprintf(buffer,"%s\r\n",buff);
						write(client_sockfd,buffer,MAX_BUFFER);
						
						if(ch==LOG_FAILED)
						{
							continue;
						}
						
						while(1)
						{							
							client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_addr, &len);
							memset(buffer,0, MAX_BUFFER+1);
		  					size = read(client_sockfd, buffer, MAX_BUFFER+1);
		  					buffer[size] = '\0';
		  					//printf("loop\n");
		  					//printf("%d\n",size);
		  					if (size<=7)  continue;
		  					buff = buffer;		  					
		  					if(buff!=0)
								{	
									printf("%s\n",buff);
								}
		  					parseJson(buff); 
		  					/*用户注销*/
			  					if(strcmp(sign,"out")==0)  	  					
				  					{
				  						loginflag=0;
				  						ch=LOG_OUT;
				  						buff=makeJson_sign(ch);
										sprintf(buffer,"%s\r\n",buff);
										write(client_sockfd,buffer,MAX_BUFFER);
				  						break;
				  					}
		  					 	if(strcmp(sign,"opendoor")==0)
									{
										opera="opendoor";
										ch= open_door();
										printf("ch:%d\n",ch);
										if(ch==OPEN_SUCCESS)
										{	
											printf("opendoor success\n");
											lupdate(username,opera);
										}
										buff=makeJson_sign(ch);
										sprintf(buffer,"%s\r\n",buff);
										write(client_sockfd,buffer,MAX_BUFFER);					
									}
								 if(strcmp(sign,"closedoor")==0)
									{
										opera="closedoor";
										ch= close_door();
										if(ch==CLOSE_SUCCESS)
										{
											lupdate(username,opera);
										}
										buff=makeJson_sign(ch);
										sprintf(buffer,"%s\r\n",buff);
										write(client_sockfd,buffer,MAX_BUFFER);
									}  
								 if(strcmp(sign,"viewlog")==0)
									{					
										bv= viewlog();
										if(bv!=NULL)
										{
											cv=3;
										}
										buff=makeJson_logfi(bv,cv);
										sprintf(buffer,"%s\r\n",buff);
										write(client_sockfd,buffer,MAX_BUFFER);
									}
								 if(strcmp(sign,"createcode")==0)
									{					
										opera="createcode";
										bc= createcode();
										if(bc!=NULL)
										{
											lupdate(username,opera); 
											cc=1;
										}
										buff=makeJson_regcode(bc,cc);
										sprintf(buffer,"%s\r\n",buff);
										write(client_sockfd,buffer,MAX_BUFFER);
									}
							}	  					
					}				
				//close(client_sockfd);
	  }
	  /* close the socket */
	  close(server_sockfd);
	      
	  return 0;
}
