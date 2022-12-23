#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<string.h>

#include "communication.h"

int send_message(int sockfd, unsigned char *message){
	int send_bytes, bytes_to_send;
	bytes_to_send = strlen((char *) message);

	while(bytes_to_send > 0){
		send_bytes = send(sockfd, (void *) message, bytes_to_send, 0);
		if(send_bytes == -1)
			return -1;
		message += send_bytes;
		bytes_to_send -= send_bytes;
	}

	return 0;
}

int receive_message(int sockd, unsigned char *buffer){
#define EOL  "\r\n"
#define EOL_SIZE 2
	unsigned char *p;
	int eol_matched = 0;
	p = buffer;

	while(recv(sockd, (void *)p, 1, 0) == 1) {
		if(*p == EOL[eol_matched]){
			eol_matched++;
			if(eol_matched == 2){
				*(p + 1 - EOL_SIZE) = '\0';
				return strlen((char *)p);
			}
		}
		else
			eol_matched = 0;
		p++;
	}
	return 0;
}
