
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sqlite3.h>
#include "cJSON.h"
#include "motorioctl.h"
#include "cccjson.h"
#include "door.h"
#include "log.h"
#include "login.h"
#include "register.h"
#include "./MyLib/DataCalc/Sha256Calc.h"
#include "./MyLib/DataCalc/Sha256Calc.c"
#define REG_SUCCESS 1
#define REG_FAILED 2
#define LOG_SUCCESS 1
#define LOG_FAILED 2
#define SIGN_SUCCESS 5
#define SIGN_FAILED 6
#define OPEN_SUCCESS 7
#define OPEN_FAILED 8
#define CLOSE_SUCCESS 9
#define CLOSE_FAILED 10
#define VIEW_LOG_SUCCESS 11
#define VIEW_LOG_FAILED 12
#define PORT 30000
#define MAX_BUFFER 2048

Sha256Calc s1;
 SZ_T x;
 
char *sign,*username,*password, *regcode;
char pwd[32];
int parseJson(char * pMsg)
{	
	if(NULL == pMsg)
	{
		return;
	}
	cJSON * pJson = cJSON_Parse(pMsg);
	if(NULL == pJson) 
	{
		return ;
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

	  /* create a socket */
	  int server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	  
	  struct sockaddr_in server_addr;
	  server_addr.sin_family = AF_INET;
	  server_addr.sin_addr.s_addr = inet_addr("192.168.2.1");
	  server_addr.sin_port = htons(PORT);
	  
	  /* bind with the local file */
	  bind(server_sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
	  
	  /* listen */
	  listen(server_sockfd, 5);
	  
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
	  		size = read(client_sockfd, buffer, MAX_BUFFER+1);
	  		printf("%d\n",size);
			buffer[size] = '\0';
			int ch ;
			char *opera;
			char *buff,*b;
			buff = buffer;
			if(buff!=0)
			{	
				printf("%s\n",buff);
			}
			parseJson(buff); 
				if (strcmp(sign,"login")==0)
					{
						memset(buffer,0, MAX_BUFFER+1);
						opera="login\n";
						 Sha256Calc_reset(&s1);
  						 Sha256Calc_calculate(&s1,password,strlen(password));
  						 for(x = 0; x < 32; x ++)
	  					{
	  						pwd[x]=( SZ_UCHAR)( s1.Value[x]);
	  					}
	  					for( x = 0; x < 32; x ++ ) printf("%x ",( SZ_UCHAR)( s1.Value[x]));
						printf("\n");
						ch=login(username,pwd);
						if(ch==LOG_SUCCESS)
						{
							lupdate(username,opera);
							buff="1";
						}
						//buff=makeJson_sign(ch);
						
						sprintf(buffer,"%s\r\n",buff);
						
					}	
				else if (strcmp(sign,"reg")==0)
				{	
					memset(buffer,0, MAX_BUFFER+1);
					opera="register\n";
					 Sha256Calc_reset(&s1);
  					Sha256Calc_calculate(&s1,password,strlen(password));
  					for(x = 0; x < 32; x ++)
  					{
  						pwd[x]=( SZ_UCHAR)( s1.Value[x]);
  					}
  					for( x = 0; x < 32; x ++ ) printf("%x ",( SZ_UCHAR)( s1.Value[x]));
					printf("\n");
					ch= regist( username,pwd, regcode);
					if(ch==REG_SUCCESS)
						{
							lupdate( username,opera);
						}
					buff=makeJson_sign(ch);
					sprintf(buffer,"%24s\r\n",buff);
				}
				else if(strcmp(sign,"viewlog")==0)
				{
					memset(buffer,0, MAX_BUFFER+1);
					b= viewlog();
					buff=makeJson_logfi(b);
					sprintf(buffer,"%24s\r\n",buff);
				}
				else if(strcmp(sign,"createcode")==0)
				{
					memset(buffer,0, MAX_BUFFER+1);
					opera="createcode";
					b= createcode();
					if(b!=NULL)
					{
						lupdate(username,opera); 
					}
					buff=makeJson_regcode(b);
					sprintf(buffer,"%24s\r\n",buff);
				}
				else if(strcmp(sign,"opendoor")==0)
				{
					opera="opendoor";
					ch= open_door();
					if(ch==OPEN_SUCCESS)
					{
						lupdate(username,opera);
					}
					buff=makeJson_sign(ch);
					sprintf(buffer,"%24s\r\n",buff);
				}
				else if(strcmp(sign,"closedoor")==0)
				{
					opera="closedoor";
					ch= close_door();
					if(ch==CLOSE_SUCCESS)
					{
						lupdate(username,opera);
					}
					buff=makeJson_sign(ch);
					sprintf(buffer,"%24s\r\n",buff);
				}
				write(client_sockfd,buffer,MAX_BUFFER);
				close(client_sockfd);
	  }
	  /* close the socket */
	  close(server_sockfd);
	      
	  return 0;
}
