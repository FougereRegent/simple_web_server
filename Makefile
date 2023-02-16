$CC=gcc
$FLAGS=-g
$BIN=sevrer_web

server_web: main.o communication.o hashtable.o files.o
	$(CC) -g -o server_web main.o communication.o hashtable.o files.o

main.o: main.c 
	$(CC) -g -Wall -c main.c 

communication.o : communication.c
	$(CC) -g -Wall -c communication.c

hashtable.o : hashtable.c
	$(CC) -g -Wall -c hashtable.c
files.o : files.c
	$(CC) -g -Wall -c files.c

clean:
	rm -r *.o
