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

typedef enum {
	OK = 200,
	NOT_FOUND = 404,
} ENUM_CODE;


static void handle_connection(int, struct sockaddr_in*);
static void wich_verb(int, char*, ENUM_VERB);
static void send_header(int, int);
static void get_verb(int, const char*);
static void put_verb(int, const char*);
static void delete_verb(int, const char*);
static void post_verb(int, const char*);
static void head_verb(int, const char*);


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
			wich_verb(sock, (char *)ptr + 1, GET);
		}
		else if(strncmp((char*) request, "PUT ", 4) == 0){
			ptr = request + 4;
			wich_verb(sock, (char *)ptr + 1, PUT);
		}
		else if(strncmp((char*) request, "DELETE ", 7) == 0) {
			ptr = request + 7;
			wich_verb(sock, (char *)ptr + 1, DELETE);
		}
		else if(strncmp((char*) request, "HEAD ", 5) == 0) {
			ptr = request + 5;
			wich_verb(sock, (char *)ptr + 1, HEAD);
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

static void wich_verb(int sock, char *route,ENUM_VERB verb)
{
	switch(verb) {
		case GET:
			get_verb(sock, route);
			break;
		case HEAD:
			head_verb(sock, route);
			break;
		case PUT:
			put_verb(sock, route);
			break;
		case DELETE: 
			delete_verb(sock, route);
			break;
		case POST: 
			post_verb(sock, route);
			break;
	}
}

static void send_header(int sock, int error_code) 
{
	const char *header = "HTTP/1.1 200 OK\r\nServer: tiny-server\r\n\r\n";
	send_message(sock, (unsigned char*)header);
}

static void get_verb(int sock, const char* route)
{

}
static void put_verb(int sock, const char* route) 
{

}
static void delete_verb(int sock, const char* route)
{

}
static void post_verb(int sock, const char* route)
{

}
static void head_verb(int sock, const char* route)
{

}
