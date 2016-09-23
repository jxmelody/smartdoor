#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include "motorioctl.h"
#define OPEN_SUCCESS 5
#define OPEN_FAILED 6
#define CLOSE_SUCCESS 7
#define CLOSE_FAILED 8
int ret,v,sta=0;
int q=1,i;
int fd,p=1;

int open_door()
{
	fd =  open("/dev/motor", O_RDWR);
	if(fd < 0)
	{
		perror("open");
		return OPEN_FAILED;
		exit(1);
		
	}
	
	for(i=0;i<=2;i++)
	{
		ret = ioctl(fd, MOTOR_CCW);//逆时钟open
	}
	if(ret < 0)
	{
		perror("open_ioctl");
		return OPEN_FAILED;
		exit(1);
	}
	sta = 1;
	sleep(6);
	//return OPEN_SUCCESS;

	/*	while(1)
		{
			v= ioctl(fd,MOTOR_GPIO,&q);//gpio_get_value(13);
			
			if(p>q)
			{
				
				break;
			}
			p=q;
		}*/
	//if (sta == 1 && q==0)
	//{
		for(i=0;i<=2;i++)
		{
			ret = ioctl(fd, MOTOR_CW);//逆时钟 close door
		}
		if(ret < 0)
		{
			perror("close_ioctl");
			return CLOSE_FAILED;
			exit(1);
		}
		sta = 0;	
	//}
	return OPEN_SUCCESS;
}

int close_door()
{	

	fd =  open("/dev/motor", O_RDWR);
	if(fd < 0)
	{
		perror("open");
		return CLOSE_FAILED;
		exit(1);
	}
/*	while(1)
	{
		v= ioctl(fd,MOTOR_GPIO,&q);//gpio_get_value(13);
		if(q==0)break;
	}

	if(v < 0)
	{
		perror("ioctl");
		return CLOSE_FAILED;
		exit(1);
	}*/
	if(q==0)
	{
		for(i=0;i<2;i++)
		{
			ret = ioctl(fd, MOTOR_CW);//逆时钟 close door
		}
		if(ret < 0)
		{
			perror("close_ioctl");
			return CLOSE_FAILED;
			exit(1);
		}
	}
	else  
	{
		perror("not match!");
		return CLOSE_FAILED;
		exit(1);
	}
	return CLOSE_SUCCESS;	
}




