$CC=gcc
$FLAGS=-g
$BIN=sevrer_web

server_web: main.o communication.o 
	$(CC) -o server_web main.o communication.o

main.o: main.c 
	$(CC) -Wall -c main.c 

communication.o : communication.c
	$(CC) -Wall -c communication.c

clean:
	rm -r *.o
