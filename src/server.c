#include "server.h"
#include "addr.h"
#include <bits/posix2_lim.h>
#include <netdb.h>
#include <stdio.h>
#include <limits.h>
#include <sys/socket.h>
#include <unistd.h>

// Function to bind to a given address and port:
// ip, port: contains the ip and port to bind to
// config: contains configuration details for address
// socketf: will contain the file descriptor for the socket
int bindAddress(const char* ip, const char* port, const struct addrinfo* config, int *socketf) {

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

	// https://beej.us/guide/bgnet/
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
		return 0;
	}

	*socketf = sockfd;
	return 1;
}

// Function to listen to a given socket
int listenAddress(int sockfd) {

	// Error handling:
	if ( listen(sockfd, 1) == -1 ) {
		printf("Couldn't listen on socket\n");
		return 0;
	}

	return 1;
}

// Function to accept a connection, and return the new file descriptor:
int acceptConn(int sockfd, int *new_fd) {
	
	// Declaring variables:
	// addr: stores connection information
	// addrlen: stores size of addr
	// new_fd: stores the new file descriptor,
	// for sending and recving
	struct sockaddr_storage addr;
	socklen_t addrlen;
	int new_f;

	// Setting variables:
	addrlen = sizeof(addr);

	// Accept and handle errors:
	if ( (new_f = accept(sockfd, (struct sockaddr *)&addr, &addrlen) ) == -1 ) {
		printf("Couldn't accept connection.\n");
   		return 0;
	}	   

	*new_fd = new_f;
	return 1;
}

// Function to receive data from a connection, and write to the given buffer
// Note: length must be equal to the amount of data to receive,
// otherwise may lead to getting stuck; TODO: fix this 
// TODO: Bug in recv_large.sh, leading to lost bytes on some systems
int receiveData(int sockfd, char *buf, long len) {
	
	// Declaring variables:
	// bytes: stores number of bytes from recv
	// total: stores the total number of bytes read
	long bytes;
	int total = 0;

	// https://stackoverflow.com/questions/30655002/socket-programming-recv-is-not-receiving-data-correctly	
	// Total number of bytes:

	// Receiving all the bytes;
	while ( 1 ) {
		bytes = recv(sockfd, buf, len, 0);
		printf("Bytes: %ld\n", bytes);
		if ( bytes == -1 ) break;
		if ( bytes == 0 ) break;
		total += bytes;
	}

	// Handling errors
	if ( bytes == -1 ) {
		printf("Couldn't receive data.\n");
		return 0;
	}

	if ( bytes == 0 ) {
		printf("Connection likely closed.\n");
	}

	return total;
}

// Function to start a server with a given config
// Returns the file descriptor if successful, -1 if not
int startServer(struct addrinfo *config, char *address, char *port) {

	// Declaring new variables:
	int sockfd, new_fd;
	
	// Creating configuration:
	if ( !createConfig(config) ) {
		printf("Couldn't create config.\n");
		return -1;
	}

	// Binding address:
	if ( !bindAddress(address, port, config, &sockfd) ) {
		printf("Couldn't bind. from main.\n");
		return -1;
	}

	// Listening for connection:
	printf("Listening for connections...\n");
	if ( !listenAddress(sockfd) ) {
		printf("Couldn't listen for connections.\n");
		return -1;
	}

	// Accept connection:
	if ( !acceptConn(sockfd, &new_fd) ) {
		printf("Couldn't accept connection.\n");
		return -1;
	}

	printf("Accepting a connection.\n");
	return new_fd;
}
