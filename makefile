LDFLAGS+=-lm -lsqlite3 -lpthread 
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/ssl/lib
servertest:$(servertest)

servertest : cccjson.o  cJSON.o door.o log.o login.o register.o 
	$(CC) $(CFLAGS) -o servertest cccjson.o  cJSON.o door.o log.o login.o  register.o serverTest1.c $(LDFLAGS) -g -I/usr/local/ssl/include -L/usr/local/ssl/lib -lssl -lcrypto -ldl
 cccjson.o:cccjson.c cccjson.h
	 $(CC) $(CFLAGS) -c -o cccjson.o cccjson.c
cJSON.o:cJSON.c cJSON.h
	$(CC) $(CFLAGS) -c -o cJSON.o cJSON.c
door.o:door.c door.h
	$(CC) $(CFLAGS) -c -o door.o door.c
log.o:log.c log.h
	$(CC) $(CFLAGS) -c -o log.o log.c
login.o:login.c login.h
	$(CC) $(CFLAGS) -c -o login.o login.c
register.o:register.c register.h
	$(CC) $(CFLAGS) -c -o register.o register.c
clean:
	rm  -f *.o servertest test.db

