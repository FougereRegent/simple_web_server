#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>
#include "communication.h"
#include "hashtable.h"

#define PORT 8080
#define MAX_BUFFER 4096
#define HEADER_SIZE 12392

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
static int create_http_table();
static void wich_verb(int, char*, ENUM_VERB);
static void send_header(int, ENUM_CODE);
static void get_verb(int, const char*);
static void put_verb(int, const char*);
static void delete_verb(int, const char*);
static void post_verb(int, const char*);
static void head_verb(int, const char*);

struct hashtable *http_table;

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
	
	if(create_http_table() < 0) {
		exit(0); 
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
			wich_verb(sock, (char *)ptr, GET);
		}
		else if(strncmp((char*) request, "PUT ", 4) == 0){
			ptr = request + 4;
			wich_verb(sock, (char *)ptr, PUT);
		}
		else if(strncmp((char*) request, "DELETE ", 7) == 0) {
			ptr = request + 7;
			wich_verb(sock, (char *)ptr, DELETE);
		}
		else if(strncmp((char*) request, "HEAD ", 5) == 0) {
			ptr = request + 5;
			wich_verb(sock, (char *)ptr, HEAD);
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

static int create_http_table() {
	int result = 0;
	http_table = create_table(70);

	result += add_element(http_table, 100, "CONTINUE");
	result += add_element(http_table, 101, "SWITCH_PROTOCOLS");
	result += add_element(http_table, 102, "PROCESSING");
	result += add_element(http_table, 103, "EARLY_HINTS");
	result += add_element(http_table, 200, "OK");
	result += add_element(http_table, 201, "CREATED");
	result += add_element(http_table, 202, "ACCEPTED");
	result += add_element(http_table, 203, "NON_AUTHORITATIVE_INFORMATION");
	result += add_element(http_table, 204, "NO_CONTENT");
	result += add_element(http_table, 205, "RESET_CONTENT");
	result += add_element(http_table, 206, "PARTIAL_CONTENT");
	result += add_element(http_table, 207, "MULTI_STATUS");
	result += add_element(http_table, 208, "ALREADY_REPORTED");
	result += add_element(http_table, 210, "CONTENT_DIFFERENT");
	result += add_element(http_table, 226, "IM_USED");
	result += add_element(http_table, 300, "MULTIPLE_CHOICES");
	result += add_element(http_table, 301, "MOVED_PERMANENTLY");
	result += add_element(http_table, 302, "FOUND");
	result += add_element(http_table, 303, "SEE_OTHER");
	result += add_element(http_table, 304, "NOT_MODIFIED");
	result += add_element(http_table, 305, "USE_PROXY");
	result += add_element(http_table, 306, "SWITCH_PROXY");
	result += add_element(http_table, 307, "TEMPORALY_REDIRECT");
	result += add_element(http_table, 308, "PERMANENT_REDIRECT");
	result += add_element(http_table, 310, "TOO_MANY_REDIRECT");
	result += add_element(http_table, 400, "BAD_REQUEST");
	result += add_element(http_table, 401, "UNAUTHORIZED");
	result += add_element(http_table, 402, "PAYMENT_REQUIRED");
	result += add_element(http_table, 403, "FORBIDDEN");
	result += add_element(http_table, 404, "NOT_FOUND");
	result += add_element(http_table, 405, "METHOD_NOT_ALLOWED");
	result += add_element(http_table, 406, "NOT_ACCEPTABLE");
	result += add_element(http_table, 407, "PROXY_AUTHENTICATION_REQUIRED");
	result += add_element(http_table, 408, "REQUEST_TIME_OUT");
	result += add_element(http_table, 409, "CONFLICT");
	result += add_element(http_table, 410, "GONE");
	result += add_element(http_table, 411, "LENGHT_REQUIRED");
	result += add_element(http_table, 412, "PRECONDITION_FAILED");
	result += add_element(http_table, 413, "REQUEST_ENTITY_TOO_LARGE");
	result += add_element(http_table, 414, "REQUEST_URI_TOO_LONG");
	result += add_element(http_table, 415, "UNSUPPORTED_MEDIA_TYPE");
	result += add_element(http_table, 416, "REQUEST_RANGE_UNSATISFIABLE");
	result += add_element(http_table, 417, "EXCEPTATION_FAILED");
	result += add_element(http_table, 418, "TEAPOT");
	result += add_element(http_table, 419, "PAGE_EXPIRED");
	result += add_element(http_table, 421, "BAD_MAPPING");
	result += add_element(http_table, 422, "UNPROCESSABLE_ENTITY");
	result += add_element(http_table, 423, "LOCKED");
	result += add_element(http_table, 424, "METHOD_FAILURE");
	result += add_element(http_table, 425, "TOO_EARLY");
	result += add_element(http_table, 426, "UPGRADE_REQUIRED");
	result += add_element(http_table, 428, "PRECONDITION_REQUIRED");
	result += add_element(http_table, 429, "TOO_MANY_REQUEST");
	result += add_element(http_table, 431, "REQUEST_HEADER_FIELDS_TOO_LARGE");
	result += add_element(http_table, 449, "RETRY_WITH");
	result += add_element(http_table, 450, "BLOCKED_BY_WINDOWS_PARENTAL_CONTROL");
	result += add_element(http_table, 451, "UNVAILABLE_FOR_LEGAL_REASON");
	result += add_element(http_table, 456, "UNRECOVERABLE_ERROR");
	result += add_element(http_table, 500, "INTERNAL_SERVER_ERROR");
	result += add_element(http_table, 501, "NOT_IMPLEMENTED");
	result += add_element(http_table, 502, "BAD_GATEWAY");
	result += add_element(http_table, 503, "SERVICE_UNAVAILABLE");
	result += add_element(http_table, 504, "GATEWAY_TIME_OUT");
	result += add_element(http_table, 505, "HTTP_VERSION_NOT_SUPPORTED");
	result += add_element(http_table, 506, "VARIANT_ALSO_NEGOTIATES");
	result += add_element(http_table, 507, "INSUFFICIENT_STORAGE");
	result += add_element(http_table, 508, "LOOP_DETECTED");
	result += add_element(http_table, 509, "BANDWITH_LIMIT_EXCEEDED");
	result += add_element(http_table, 510, "NOT_EXTENDED");
	result += add_element(http_table, 511, "NETWORK_AUTHENTIFICATION_REQUIRED");

	if(result < 0) {
		free_table(http_table);
		return -1;
	}
	return 0;
}

static void wich_verb(int sock, char *route,ENUM_VERB verb) {
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

static void send_header(int sock, ENUM_CODE error_code)  {
	struct element *item;
	char res[HEADER_SIZE];
	const char *header = "Server: tiny-server\r\n"
						"Author: Venant-Valery Damien\r\n\r\n\r\n";

	memset((char *)res, 0, HEADER_SIZE);
	item = get_item(http_table, error_code);
	
	sprintf(res, "HTTP/1.1 %d %s\r\n%s", 
			error_code, 
			item->value, 
			header);
	send_message(sock, (unsigned char*)res);
}

static void get_verb(int sock, const char* route) {
	int route_size = strlen(route);
	if(strncmp(route, "/", route_size) == 0) {
	}
}
static void put_verb(int sock, const char* route) {

}
static void delete_verb(int sock, const char* route) {

}
static void post_verb(int sock, const char* route) {

}
static void head_verb(int sock, const char* route) {

}
