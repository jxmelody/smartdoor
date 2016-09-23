#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<string.h>
#include <sqlite3.h>
#define LOG_SUCCESS 1
#define LOG_FAILED 2
static int callback(void *data, int argc, char **argv, char **azColName){
   int i;
   fprintf(stderr, "%s: ", (const char*)data);
   for(i=0; i<argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}

int login(char *u_n,char *p_w)
{	
	char *sql1; 
	char *sql2; 
	  int i,j;
	char psql1[1024];
	char psql2[1024];
	sql1=psql1;
	sql2=psql2;
	int log_flag = LOG_FAILED,userexit;
	   int rc,nRow,nColumn,result,index;
	   char *zErrMsg = 0;
	    char **dbResult;
	  /* Open database */
	  char *username=u_n;
	  char *password=p_w;
	  char *pwd;
	  const char* data = "Callback function called";
	  sqlite3 *db;
	   rc = sqlite3_open("test.db", &db);
	   if( rc )
	   {
	      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
	      exit(0);
	   }else{
	      fprintf(stderr, "Opened database successfully\n");
 	  }
 	    /* Create SQL statement */
	   strcpy(sql1, "SELECT USERNAME  FROM USER ");
	   result= sqlite3_get_table(db, sql1, &dbResult, &nRow,&nColumn, &zErrMsg);
	   if( result != SQLITE_OK ){
	            fprintf(stderr, "SQL error: %s\n", zErrMsg);
     		    sqlite3_free(zErrMsg);
	   }
	   else
	   {
		   index=nColumn;
		   for(i=0;i<nRow;i++)
		   {
		   	for(j=0;j<nColumn;j++)
		   	{
		   		printf("%s,%s\n",dbResult[j],dbResult[index]);
		   		/*判断用户名是否匹配*/
		   		if(strcmp(dbResult[index],username)==0)
		   		{
		   			printf("username exit\n");
		   			userexit=1;
		   			break;
		   		}
		   		else 
		   		{
		   			log_flag =LOG_FAILED;
		   			index++;
		   		}
		   	}
		   }
	  }
		   if(userexit=1)
		   {
			   strcpy(sql1, "SELECT PASSWORD  FROM USER where USERNAME LIKE\'");
			   strcat(sql1,username);
			   strcat(sql1,"\';");
			   result= sqlite3_get_table(db, sql1, &dbResult, &nRow,&nColumn, &zErrMsg);
			   if( result != SQLITE_OK ){
			   fprintf(stderr, "SQL error: %s\n", zErrMsg);
	     		    sqlite3_free(zErrMsg);
	     		    }
	     		    else
	     		    {
		     		index=nColumn;
				for(i=0;i<nRow;i++)
				{
				   	for(j=0;j<nColumn;j++)
				   	{
			   			printf("%s,%s\n",dbResult[j],dbResult[index]);
			   			/*判断密码是否匹配*/
				   		if(strcmp(dbResult[index],password)==0)
					   	{
								 log_flag = LOG_SUCCESS;
								 continue;
					   	}
						else 
					   	{
					   			log_flag =LOG_FAILED;
					   			printf("password error!\n"); 	
					   	}
					 }
				}	
			}
	  	 } 
	  	 else printf("username error!\n");   	
	 memset(psql1,0,1024);
	 sqlite3_close(db);
	printf("log_flag:%d",log_flag);
	return log_flag;
}
