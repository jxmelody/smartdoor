

#ifndef register_h
#define register_h
#endif
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>


extern int regist(char *u_n,char *p_w,char *r_c);
extern char* createcode(void);
extern int judgecode(char *buffer);

