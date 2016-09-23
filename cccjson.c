#include <stdio.h> 
#include<stdlib.h>
#include<string.h> 
#include "cJSON.h"

struct jsonvalue{
	char sign[10];
	char username[20];
	char password[20];
	char regcode[20];
	};


char * makeJson_regcode(char *r,int c)
{
	cJSON * pJsonRoot = NULL;
	char *regcode;
	int createflag;
	pJsonRoot = cJSON_CreateObject();
	if(NULL == pJsonRoot)
	{
		//error happend here
		return NULL;
	}
	regcode=r;
	createflag=c;
	cJSON_AddNumberToObject(pJsonRoot, "result",createflag);
	cJSON_AddStringToObject(pJsonRoot, "value",regcode);
	char * p = cJSON_Print(pJsonRoot);
	if(NULL == p)
	{
		cJSON_Delete(pJsonRoot);
		return NULL;
	}
	cJSON_Delete(pJsonRoot);
	return p;
}

char * makeJson_logfi(char *l,int c)
{
	cJSON * pJsonRoot = NULL;
	char *logfile;
	int logflag;
	pJsonRoot = cJSON_CreateObject();
	if(NULL == pJsonRoot)
	{
		//error happend here
		return NULL;
	}
	logfile=l;
	logflag=c;
	cJSON_AddNumberToObject(pJsonRoot, "result",logflag);
	cJSON_AddStringToObject(pJsonRoot, "value",logfile);
	char * p = cJSON_Print(pJsonRoot);
	if(NULL == p)
	{
		cJSON_Delete(pJsonRoot);
		return NULL;
	}
	cJSON_Delete(pJsonRoot);
	return p;
}

char * makeJson_sign(int r_s)
{
	cJSON * pJsonRoot = NULL;
	int return_sign=0;
	pJsonRoot = cJSON_CreateObject();
	if(NULL == pJsonRoot)
	{
		//error happend here
		return NULL;
	}
	return_sign=r_s;
	cJSON_AddNumberToObject(pJsonRoot, "result",return_sign);
	char * p = cJSON_Print(pJsonRoot);
	if(NULL == p)
	{
		cJSON_Delete(pJsonRoot);
		return NULL;
	}
	cJSON_Delete(pJsonRoot);
	return p;
}
