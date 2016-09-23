
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sqlite3.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include "cJSON.h"
#include "motorioctl.h"
#include "cccjson.h"
#include "door.h"
#include "log.h"
#include "login.h"
#include "register.h"
#include "./MyLib/DataCalc/Sha256Calc.h"
#include "./MyLib/DataCalc/Sha256Calc.c"
#define BACKLOG 5
#define REG_SUCCESS 1
#define REG_FAILED 2
#define LOG_SUCCESS 1
#define LOG_FAILED 2
#define SIGN_SUCCESS 1
#define SIGN_FAILED 2
#define OPEN_SUCCESS 1
#define OPEN_FAILED 2
#define CLOSE_SUCCESS 1
#define CLOSE_FAILED 2
#define VIEW_LOG_SUCCESS 1
#define VIEW_LOG_FAILED 2
#define PORT 30000
#define MAX_BUFFER 2048

	Sha256Calc s1;
 	SZ_T x;
 	char pwd[32];
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
	
int main(int argc, char **argv)
{
	int ch=0 ;
	int cv=2;
	int cc=2;

	socklen_t size;  
	char buffer[MAX_BUFFER + 1];
	char *opera,*buff,*bc,*bv;
	buff = buffer;
	  int client_sockfd,server_sockfd ;
	  struct sockaddr_in server_addr;
	  struct sockaddr_in client_addr;
	//  socklen_t len = sizeof(client_addr);
	  SSL_CTX *ctx;
	  SSL *ssl;
	if (argc != 3) 
	{
		printf("参数格式错误!正确用法如下:\n\t\t%s 数字证书 密钥文件\n\t 比如:\t%s cacert.pem privkey.pem \n ",argv[0], argv[0]);
		exit(0);
	}
	/* SSL 库初始化*/
	SSL_library_init();
	/* 载入所有 SSL 算法 */
	OpenSSL_add_all_algorithms();
	/* 载入所有 SSL 错误消息 */
	SSL_load_error_strings();
	/* 以 SSL V2 和 V3 标准兼容方式产生一个 SSL_CTX ,即 SSL Content Text */
	ctx = SSL_CTX_new(SSLv23_server_method());
	/* 也可以用 SSLv2_server_method() 或 SSLv3_server_method() 单独表示 V2 或 V3标准 */
	if (ctx == NULL)
	 {
		ERR_print_errors_fp(stdout);
		exit(1);
	}
	/* 载入用户的数字证书, 此证书用来发送给客户端。 证书里包含有公钥 */
	if (SSL_CTX_use_certificate_file(ctx, argv[1], SSL_FILETYPE_PEM) <= 0) 
	{
		ERR_print_errors_fp(stdout);
		exit(1);
	}
	/* 载入用户私钥 */
	if (SSL_CTX_use_PrivateKey_file(ctx, argv[2], SSL_FILETYPE_PEM) <= 0) 
	{
		ERR_print_errors_fp(stdout);
		exit(1);
	}
	/* 检查用户私钥是否正确 */
	if (!SSL_CTX_check_private_key(ctx)) 
	{
		ERR_print_errors_fp(stdout);
		exit(1);
	}
	/* 开启一个 socket 监听 */
	  if(( server_sockfd = socket(AF_INET, SOCK_STREAM, 0))==-1)
	  {
	  	perror("socket");
		exit(1);
	  }else
		printf("socket created\n");
	  /*初始化地址和端口信息 */
	   bzero(&server_addr, sizeof(server_addr));
	  server_addr.sin_family = AF_INET;
	//  server_addr.sin_addr.s_addr = inet_addr("192.168.2.1");
	 server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	  server_addr.sin_port = htons(PORT);
	  
	  /* bind with the local file */
	  if(bind(server_sockfd, (struct sockaddr *)&server_addr, sizeof( server_addr))==-1)
	  {
	  	perror("bind");
	  	exit(errno);
	  }else
	  	printf("bind success\n");

	  /* listen */
	  if(listen(server_sockfd, BACKLOG)==-1)
	  {
	  	perror("listen");
	  	exit(errno);
	  }else
	  	printf ("begin listen\n");	  
	  /* accept a connection */
	  printf("waiting connection...\n");
	
	  	
	  while(1)
	  {		
	  		SSL *ssl;	  		
			size = sizeof(struct sockaddr);
	  		if((client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_addr, &size))==-1)
	  		{
	  			perror("accept");
				exit(errno);
			}else
			printf("server: got connection from %s, port %d, socket %d\n",inet_ntoa( server_addr.sin_addr),ntohs( server_addr.sin_port), client_sockfd);
			/* 基于 ctx 产生一个新的 SSL */
			ssl = SSL_new(ctx);
			/* 将连接用户的 socket 加入到 SSL */
			SSL_set_fd(ssl, client_sockfd);
			/* 建立 SSL 连接 */
			if (SSL_accept(ssl) == -1) 
			{
				perror("accept");
				close(client_sockfd);
				break;
			}
			printf("connection established!\n");
			memset(buffer,0, MAX_BUFFER);
			/* 接收客户端的消息 */
	  		size = SSL_read(ssl, buffer, MAX_BUFFER);
	  		if (size > 0)
				printf("接收消息成功:'%s',共%d 个字节的数据\n",buffer, size);
			else {
			printf("消息接收失败!错误代码是%d,错误信息是'%s'\n",errno, strerror(errno));
			goto finish;
			}
			buffer[size] = '\0';
			if(buff!=0)
			{	
				printf("%s\n",buff);
			}
			 
			parseJson(buff); 
				if (strcmp(sign,"login")==0)
					{
						
						ch=0;
						opera="login";
						 Sha256Calc_reset(&s1);
  						 Sha256Calc_calculate(&s1,password,strlen(password));
  						 for(x = 0; x < 32; x ++)
	  					{
	  						pwd[x]=( SZ_UCHAR)( s1.Value[x]);
	  					}
	  					for( x = 0; x < 32; x ++ ) printf("%x ",( SZ_UCHAR)( s1.Value[x]));
						printf("\n");
						printf("%s\n",pwd);
						ch=login(username,pwd);
						printf("ch:%d\n",ch);
						if(ch==LOG_SUCCESS)
						{
							lupdate(username,opera);
						}
						buff=makeJson_sign(ch); 
						sprintf(buffer,"%s\r\n",buff);
						SSL_write(ssl,buffer,strlen(buffer));
						
					}	
				 if (strcmp(sign,"reg")==0)
				{	
					
					opera="register";
					Sha256Calc_reset(&s1);
  					Sha256Calc_calculate(&s1,password,strlen(password));
  					for(x = 0; x < 32; x ++)
  					{
  						pwd[x]=( SZ_UCHAR)( s1.Value[x]);
  					}
  					for( x = 0; x < 32; x ++ ) printf("%x ",( SZ_UCHAR)( s1.Value[x]));
					printf("\n");
					printf("%s\n",pwd);
					ch= regist( username,pwd, regcode);
					if(ch==REG_SUCCESS)
						{
							lupdate( username,opera);
						}
					buff=makeJson_sign(ch);
					sprintf(buffer,"%s\r\n",buff);
					SSL_write(ssl,buffer,strlen(buffer));
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
					SSL_write(ssl,buffer,strlen(buffer));
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
					SSL_write(ssl,buffer,strlen(buffer));
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
					SSL_write(ssl,buffer,strlen(buffer));					
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
					SSL_write(ssl,buffer,strlen(buffer));
				}
				/* 处理每个新连接上的数据收发结束 */
				finish:
				/* 关闭 SSL 连接 */
				SSL_shutdown(ssl);
				/* 释放 SSL */
				SSL_free(ssl);
				/* 关闭 socket */
				close(client_sockfd);
	  }
	  /* close the socket */
	  close(server_sockfd);
	  SSL_CTX_free(ctx);
	  return 0;
}
