
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<string.h>
#include <sqlite3.h>
#define REG_SUCCESS 01
#define REG_FAILED 02
#define SIGN_SUCCESS 05
#define SIGN_FAILED 06


int regist(char *u_n,char *p_w,char *r_c)
{	
	char *sql,*sqls,*sql0; 
	char psql[1024];
	sql=psql;
	sql0=psql;
	int i,n;
	int rc;
	char *zErrMsg = 0;
	 sqlite3 *db;
	  /* Open database */
	  char *regcode=r_c;
	  char *username=u_n;
	  char *password=p_w;
	  char *pwd;
	  char salt[21];
	  int jflag = SIGN_FAILED;
	  int rflag=REG_FAILED;
	  jflag = judgecode(regcode);
	  if(  jflag!=SIGN_SUCCESS)
	  {
	  	return jflag;
	  	exit(1);
	  }
	  else
		  printf("regcode right!!\n");
	  /*pwd=hash(password)*/
	  pwd=password;
	  //SALT
	srand(time(NULL));	//以现在的系统时间作为随机数的种子来产生随机数
	for(i=0;i<20;i++)
	{
		n = rand()%36;		//产生随机数
		if(n > 25)
			salt[i] = n + '0';
		else
			salt[i] = n + 'A';
	}
	salt[21] = '\0';
	  printf("salt:%s\n",salt);
	rc = sqlite3_open("test.db", &db);
	if( rc )
	   {
	      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
	      exit(0);
	   }else{
	      fprintf(stderr, "Opened database successfully\n");
 	  }
 	  sqls="SELECT * FROM USER ";
	/* Execute SQL statement */
	rc = sqlite3_exec(db, sqls, 0, 0, &zErrMsg);
	if( rc != SQLITE_OK )
	{
	 	
 	  /* Create SQL statement */
	  	 sql0 = "CREATE TABLE USER("  \
		 "USERNAME CHAR(64) PRIMARY KEY     NOT NULL," \
		 "PASSWORD         TEXT(64)     NOT NULL," \
		 "SALT        CHAR(21) NOT NULL);";

		   /* Execute SQL statement */
		   rc = sqlite3_exec(db, sql0, 0, 0, &zErrMsg);
		   if( rc != SQLITE_OK ){
		   fprintf(stderr, "SQL error: %s\n", zErrMsg);
		      sqlite3_free(zErrMsg);
		   }else{
		      fprintf(stdout, "Table created successfully\n");
		   }
		memset(psql,0,1024);
	}

 	    /* Create SQL statement */
	   strcpy(sql, "INSERT  INTO  USER (USERNAME , PASSWORD , SALT) VALUES (\'\0");
	   strcat(sql,username);
	   strcat(sql, "\',\'");
	    strcat(sql,password);
	    strcat(sql,"\',\'");
	    strcat(sql,salt);
	    strcat(sql,"\');");

	   /* Execute SQL statement */
	   rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);
	   if( rc != SQLITE_OK ){
		   fprintf(stderr, "SQL error: %s\n", zErrMsg);
		   rflag=REG_FAILED;
		   sqlite3_free(zErrMsg);
		   }else{
		      fprintf(stdout, "insert successfully\n");
		      rflag=REG_SUCCESS;
		   }
  		sqlite3_close(db);
  		return rflag;
}






/*********************************************************
	char* createcode() :  Randomly generated length 16 registration code
return:registration code
*********************************************************/

char* createcode(void)
{
	int n;
	int i;
	int fd;
	int tmp;
	static char  buffer[17]={0};
	srand(time(NULL));	//以现在的系统时间作为随机数的种子来产生随机数
	for(i=0;i<16;i++)
	{
		n = rand()%36;		//产生随机数
		if(n > 25)
			buffer[i] = n + '0';
		else
			buffer[i] = n + 'A';
	}
	buffer[17]= ' ';

	/* 打开源文件 */  
	fd = open("regcode.txt",O_WRONLY ) ;
	if (fd  == -1) 
	{  
		printf("Open regcode.txt Error !\n");  
		exit(1);  
	}  

	/*将数据写入已打开的文件内*/
	tmp = write(fd,buffer,16);
	if(tmp == -1)
	{  
		close(fd);
		printf("Write to regcode.txt Error !\n");  
		exit(1);  
	} 
	 
	close(fd);
	return buffer;
}

/********************************************************
	int judgecode(char* buffer)   
buffer: length:16      save  registration code
return: 0    ture
		 -1   flase	
*********************************************************/
int judgecode(char *buffer)
{
	int fd;
	int i;
	int tmp;
	int jflag = SIGN_FAILED;
	char buffer1[17]={0};

	/* 打开源文件 */  
	fd = open("regcode.txt",O_RDONLY ) ;
	if (fd == -1) 
	{  
		printf("Open regcode.txt Error !\n");  
		exit(1);  
	} 

	/*读取文件内容*/
	tmp = read(fd,buffer1,16);
	if(tmp == -1)
	{  
		close(fd);
		printf("Read from regcode.txt Error !\n");  
		exit(1);  
	} 
	buffer1[17] = ' ';
 
	close(fd);

	/* 比较*/
	for(i=0;i < 16;i++)
	{
		if(buffer[i] != buffer1[i])
		{
			return -1;
			exit(1);
		}
	}
	FILE *fp;
	fp=fopen("regcode.txt","w");
	fclose(fp);
	jflag = SIGN_SUCCESS;
	
	return jflag;
}










