/*update log file*/
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include<time.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <sqlite3.h>
#include <stdbool.h>
#include "cJSON.h"
#define VIEW_LOG_SUCCESS 11
#define VIEW_LOG_FAILED 12


/*判断数据库表是否存在*/
 bool isTableExist()
{      
	int rc;
	sqlite3 *db;
	char *sql;
	char psql[100];
	sql=psql;
	char *zErrMsg = 0;
	bool TableExist=true;  
	const char* data = "Callback function called";
	rc = sqlite3_open("test.db", &db);
	if( rc )
	{
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		exit(0);
	}
	else
	{
		fprintf(stderr, "Opened database successfully-2\n");
	}
	sql="SELECT * FROM LOGFILE ";
	/* Execute SQL statement */
	rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);
	if( rc != SQLITE_OK )
	{
	 	TableExist=false;  
	 	printf("table not exist\n");
	}
	sqlite3_close(db);
	return TableExist;  
 }  


int lupdate(char *u_n , char *opera)
{
	sqlite3 *db;
	time_t now;
	char time_now[30];
	char *username=u_n;
	char *operation=opera;
	char *zErrMsg = 0;
	int rc;
	char *sql,*sql2;
	char psql[1024];
	sql=psql;
	sql2=psql;
	bool tableflag=false;
	rc = sqlite3_open("test.db", &db);
	   if( rc )
	   {
	      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
	      exit(0);
	   }else{
	      fprintf(stderr, "Opened database successfully-1\n");
 	  }
 	    /* Create SQL statement */
 	    
 	   tableflag =isTableExist();
 	   if (!tableflag )
 	   {
 	   	 /* Create SQL statement */
	  	 sql = "CREATE TABLE LOGFILE("  \
		 "TIME TEXT PRIMARY KEY     NOT NULL," \
		 "USERNAME         CHAR(64)     NOT NULL," \
		 "OPERATION        TEXT);";

		   /* Execute SQL statement */
		   rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);
		   if( rc != SQLITE_OK ){
		   fprintf(stderr, "SQL error: %s\n", zErrMsg);
		      sqlite3_free(zErrMsg);
		   }else{
		      fprintf(stdout, "Table created successfully\n");
		   }
 	   }
 	   now = time(NULL);//present time
 	   sprintf(time_now,"%s\r\n",ctime(&now));
 	   	memset(psql,0,1024);
 	   //datetime('now',NNN.NNNN seconds)
	    strcpy(sql2, "INSERT INTO LOGFILE (TIME, USERNAME, OPERATION) VALUES (\'\0");
	   strcat(sql2,time_now);
	   strcat(sql2, "\',\'");
	   strcat(sql2,username);
	   strcat(sql2, "\',\'");
	   strcat(sql2, operation);
	   strcat(sql2, "\');");

	   /* Execute SQL statement */
	   rc = sqlite3_exec(db, sql2, 0, 0, &zErrMsg);
	   if( rc != SQLITE_OK ){
	      fprintf(stderr, "SQL error: %s\n", zErrMsg);
	      sqlite3_free(zErrMsg);
	   }else{
	      fprintf(stdout, "Operation done successfully\n");
	   }
	   sqlite3_close(db);
}

/*查看日志*/
/*char *viewlog() 
 {
 	  int rc,logfd,tmp,nRow,nColumn,result,index;
 	  int i,j;
 	  sqlite3 *db;
	  char *sql;
	  char psql[100];
	  sql=psql;
	  char *zErrMsg = 0;
	  char **dbResult;
 	  int lflag=VIEW_LOG_FAILED;
 	 static char buff[1025]={0};
           rc = sqlite3_open("test.db", &db);
	   if( rc )
	   {
		   fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		   exit(0);
	   }
	   else
	   {
	  	  fprintf(stderr, "Opened database successfully\n");
 	   }
	
	 sql= " select * from LOGFILE LIMIT 10";
           /* Execute SQL statement *
	  result= sqlite3_get_table(db, sql, &dbResult, &nRow,&nColumn, &zErrMsg);
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
		   		strcat(buff,dbResult[j]);
		   		strcat(buff,"\n");
		   		strcat(buff,dbResult[index]);
		   		strcat(buff,"\n");
		   		index++;
		   	}
		   }
	   } 
	   sqlite3_free_table( dbResult );
	   sqlite3_close(db); 
	   return buff;
 }
 */
 
 char *viewlog() 
 {
 	static char buff[1025]={0};
 	 strcpy(buff,"{\"result\":3,\"value\":[{\"time\":\"2016.06.02.15:00\",\"username\":\"o\",\"operation\":\"login\"},{\"time\":\"2016.06.02.15:00\",\"username\":\"o\",\"operation\":\"opendoor\"}]}");
 	 printf("%s\n",buff);
 	 return buff;
 }
