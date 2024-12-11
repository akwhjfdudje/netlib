#include "client.h"
#include "addr.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int connectAddress(const char* ip, const char* port, const struct addrinfo* config, int *socketf) {

    // Declaring variables:
    // r: stores the pointer to loop with
    // res: stores the results of querying for addresses
    struct addrinfo *r;
    struct addrinfo *res;
	int sockfd, yes = 1;

    // Getting address results
    if ( !(getAddresses(ip, port, config, &res)) ) {
        printf("Couldn't get address results.\n");
        return 0;
    }

    // Looping through every element to get the first valid result:
    for ( r = res; r != NULL; r = r->ai_next ) {
        
        // Getting socket 
		if ( (sockfd = socket(r->ai_family, r->ai_socktype, r->ai_protocol)) == -1 ) {
			continue;
		}

		// Connecting to the socket:
		if ( connect(sockfd, r->ai_addr, r->ai_addrlen) == -1 ) {
			close(sockfd);
			continue;
		}

		break;
    }

	freeaddrinfo(res);
	
	// Exit if failed to bind:
	if ( r == NULL ) {
		printf("Couldn't bind\n");
		return 0;
	}

	*socketf = sockfd;
	return 1;
}

int sendData(int sockfd, char* msg) {

	// Getting length of message:
	int len = strlen(msg);

	// Getting number of bytes sent:
	int bytes = send(sockfd, msg, len, 0);

	// Handling:
	if ( bytes == -1 ) {
		printf("Couldn't send message.\n");
		return 0;
	}

	return 1;
}
