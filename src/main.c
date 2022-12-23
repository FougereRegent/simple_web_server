#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>
#include "communication.h"

#define PORT 8080
#define MAX_BUFFER 4096

typedef enum {
	GET,
	HEAD,
	DELETE,
	PUT,
	POST
} ENUM_VERB;


static void handle_connection(int, struct sockaddr_in*);
static void wich_verb(ENUM_VERB);

int main(int argc, char **argv){

	/*Configure socket*/
	int sock, csock, yes = 1;
	unsigned int sinsize; 
	
	struct sockaddr_in sin = { 
		.sin_addr.s_addr = htonl(INADDR_ANY),
		.sin_family = AF_INET,
		.sin_port = htons(PORT),
	};
	struct sockaddr_in csin = { 0 };
	
	memset((void*) sin.sin_zero, 0, (unsigned long)8);

	sock = socket(AF_INET, SOCK_STREAM, 0);

	if(sock == -1){
		perror("socket() : ");
		exit(errno);
	}
	if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (int*) &yes, sizeof(int)) == -1) {
		perror("setsockopt() : ");
		exit(errno);
	}
	if(bind(sock, (struct sockaddr*) &sin, sizeof(sin)) == -1){
		perror("bind(): ");
		exit(errno);
	}
	if(listen(sock, 5) == -1){
		perror("listen() : ");
		exit(errno);
	}
	fprintf(stdout, "Web server ....\n");
	sinsize = sizeof(struct sockaddr_in);
	while(1) {
		csock = accept(sock, (struct sockaddr*) &csin, &sinsize);
	
		if(csock == -1){
			perror("accept(): ");
			exit(errno);
		}
		handle_connection(csock, &csin);
	}
	
	return EXIT_SUCCESS;
}

static void handle_connection(int sock, struct sockaddr_in* addr) {
	
	unsigned char *ptr, request[MAX_BUFFER], ressource[MAX_BUFFER];
	int size;

	size = receive_message(sock, request);
	ptr = (unsigned char*)strstr((char*)request, "HTTP/"); //Permet de rechercher une requête valide
	
	fprintf(stdout, "%s\n", request);

	if(ptr != NULL) {
		*ptr = 0;
		if(strncmp((char*) request, "GET ", 4) == 0) {
			ptr = request + 4;
		}
		else if(strncmp((char*) request, "PUT ", 4) == 0){
			ptr = request + 4;
		}
		else if(strncmp((char*) request, "DELETE ", 7) == 0) {
			ptr = request + 7;
		}
		else if(strncmp((char*) request, "HEAD ", 5) == 0) {
			ptr = request + 5;
		}
		else {
			ptr = NULL;
		}

		if(ptr != NULL) {

		}
	}
	else {
		fprintf(stdout, "Error Not HTTP request");
	}

	shutdown(sock, SHUT_RDWR);
}

static void wich_verb(ENUM_VERB verb)
{
	switch(verb) {
		case GET: break;
		case HEAD: break;
		case PUT: break;
		case DELETE: break;
		case POST: break;
	}
}
