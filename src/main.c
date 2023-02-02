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
	CONTINUE = 100,
	SWITCH_PROTOCOLS = 101,
	PROCESSING = 102,
	EARLY_HINTS = 103,
	OK = 200,
	CREATED = 201,
	ACCEPTED = 202,
	NON_AUTHORITATIVE_INFORMATION = 203,
	NO_CONTENT = 204,
	RESET_CONTENT = 205,
	PARTIAL_CONTENT = 206,
	MULTI_STATUS = 207,
	ALREADY_REPORTED = 208,
	CONTENT_DIFFERENT = 210,
	IM_USED = 226,
	MULTIPLE_CHOICES = 300,
	MOVED_PERMANENTLY = 301,
	FOUND = 302,
	SEE_OTHER = 303,
	NOT_MODIFIED = 304,
	USE_PROXY = 305,
	SWITCH_PROXY = 306,
	TEMPORALY_REDIRECT = 307,
	PERMANENT_REDIRECT = 308,
	TOO_MANY_REDIRECT = 310,
	BAD_REQUEST = 400,
	UNAUTHORIZED = 401,
	PAYMENT_REQUIRED = 402,
	FORBIDDEN = 403,
	NOT_FOUND = 404,
	METHOD_NOT_ALLOWED = 405,
	NOT_ACCEPTABLE = 406,
	PROXY_AUTHENTICATION_REQUIRED = 407,
	REQUEST_TIME_OUT = 408,
	CONFLICT = 409,
	GONE = 410,
	LENGHT_REQUIRED = 411,
	PRECONDITION_FAILED = 412,
	REQUEST_ENTITY_TOO_LARGE = 413,
	REQUEST_URI_TOO_LONG = 414,
	UNSUPPORTED_MEDIA_TYPE = 415,
	REQUEST_RANGE_UNSATISFIABLE = 416,
	EXCEPTATION_FAILED = 417,
	TEAPOT = 418,
	PAGE_EXPIRED = 419,
	BAD_MAPPING = 421,
	UNPROCESSABLE_ENTITY = 422,
	LOCKED = 423,
	METHOD_FAILURE = 424,
	TOO_EARLY = 425,
	UPGRADE_REQUIRED = 426,
	PRECONDITION_REQUIRED = 428,
	TOO_MANY_REQUEST = 429,
	REQUEST_HEADER_FIELDS_TOO_LARGE = 431,
	RETRY_WITH = 449,
	BLOCKED_BY_WINDOWS_PARENTAL_CONTROL = 450,
	UNVAILABLE_FOR_LEGAL_REASON = 451,
	UNRECOVERABLE_ERROR = 456,
	INTERNAL_SERVER_ERROR = 500,
	NOT_IMPLEMENTED = 501,
	BAD_GATEWAY = 502,
	SERVICE_UNAVAILABLE = 503,
	GATEWAY_TIME_OUT = 504,
	HTTP_VERSION_NOT_SUPPORTED = 505,
	VARIANT_ALSO_NEGOTIATES = 506,
	INSUFFICIENT_STORAGE = 507,
	LOOP_DETECTED = 508,
	BANDWITH_LIMIT_EXCEEDED = 509,
	NOT_EXTENDED = 510,
	NETWORK_AUTHENTIFICATION_REQUIRED = 511

} ENUM_CODE;


static void handle_connection(int, struct sockaddr_in*);
static void wich_verb(int, char*, ENUM_VERB);
static void send_header(int, ENUM_CODE);
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

static void send_header(int sock, ENUM_CODE error_code) 
{
	const char *header = "Server: tiny-server\r\n\r\n"
						"Author: Venant-Valéry Damien";

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
