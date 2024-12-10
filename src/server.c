#include "server.h"
#include "addr.h"
#include <stdio.h>
#include <limits.h>

// Function to bind to a given address and port:
// ip, port: contains the ip and port to bind to
// config: contains configuration details for address
int bindAddress(const char* ip, const char* port, const struct addrinfo* config, int *socketf) {

    // Declaring variables:
    // r: stores the pointer to loop with
    // res: stores the results of querying for addresses
    struct addrinfo *r;
    struct addrinfo **res;
	int sockfd, new_fd, yes = 1;

    // Getting address results
    if ( !(getAddresses(ip, port, config, res)) ) {
        printf("Couldn't get address results.\n")
        return 0;
    }

    // Looping through every element to get the first valid result:
    for ( r = res; r != NULL; r = r->ai_next ) {
        
        // Getting socket 
		if ( (sockfd = socket(r->ai_family, r->ai_socktype, r->ai_protocol)) == -1 ) {
			//printf("Couldn't get socket\n");
			continue;
		}

		// Setting socket options 
		if ( setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1 ) {
			//printf("Couldn't get socket options\n");
			continue;
		}

		// Binding the socket:
		if ( bind(sockfd, r->ai_addr, r->ai_addrlen) == -1 ) {
			//printf("Couldn't bind address\n");
			close(sockfd);
			continue;
		}

		break;
    }

	freeaddrinfo(res);
	
	// Exit if failed to bind:
	if ( r == NULL ) {
		printf("Couldn't bind\n");
		return 1;
	}

	*socketf = sockfd;
}

// Function to bind to listen to a given socket
int listenAddress(int sockfd) {

	// Error handling:
	if ( listen(sockfd, 1) == -1 ) {
		printf("Couldn't listen on socket\n");
		return 1;
	}
}

// Function to accept a connection:
int acceptConn(int sockfd) {
	
	// Declaring variables:
	// addr: stores connection information
	// addrlen: stores size of addr
	// new_fd: stores the new file descriptor,
	// for sending and recving
	struct sockaddr_storage addr;
	socklen_t addrlen;
	int new_fd;

	// Setting variables:
	addrlen = sizeof(addr);

	// Accept and handle errors:
	if ( (new_fd = accept(sockfd, (struct sockaddr *)&addr, &addrlen) ) == -1 ) {
		printf("Couldn't accept connection.\n");
   		return 0;
	}	   

	return new_fd;
}

// Function to receive data from a connection
int receiveData(int sockfd) {
	
	// Storing the number of bytes received:
	int bytes;
	
	// Receiving data:
	bytes = recv(sockfd, stdout, LINE_MAX, 0);

	// Handling errors
	if ( bytes == -1 ) {
		printf("Couldn't receive data.\n");
		return 0;
	}

	if ( bytes == 0 ) {
		printf("Connection likely closed.\n");
		return 0;	
	}

	return bytes;
}
