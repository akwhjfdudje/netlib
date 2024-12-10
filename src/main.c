#include "addr.h"
#include "server.h"
#include <stdio.h>
#include <arpa/inet.h>
#include <limits.h>

// TODO:
//      implement other functions
//      create more tests for functions

// Testing code
#ifdef TEST
int main(int argc, char **argv) {
    
    // Declaring variables for first test:
    // address: stores address string to test conversion
    // v: stores ip version
    // sockaddr: stores sockaddr_in struct
    char* address = "127.0.0.1";
    int v = 0;
    struct sockaddr_in sockaddr;

    // Test for getIPToInteger in addr.h
    printf("'getIPToInteger' test result: %d\n", getIPToInteger(v, address, &(sockaddr.sin_addr)));
    
    // Declaring variables for second test:
    // ip: will store ip from function
    char ip[INET_ADDRSTRLEN];

    // Test for getIntegerToIP in addr.h
    printf("'getIntegerToIP' test result: %d\n", getIntegerToIP(v, &(sockaddr.sin_addr), ip, INET_ADDRSTRLEN));
    printf("ip from last test: %s\n", ip);

	// Test to bind and listen on an address:
	// Declaring variables:
	// sockfd: stores the file descriptor for the binding
	// new_fd: stores the file descriptor for sending and receiving
	// config: stores the configuration for the binding
	int sockfd, new_fd;
	struct addrinfo config;
	char buf[LINE_MAX];
	
	// Creating configuration:
	if ( !createConfig(&config) ) {
		printf("Couldn't create config.\n");
		return 1;
	}

	// Binding address:
	if ( !bindAddress(address, "9001", NULL, &sockfd) ) {
		printf("Couldn't bind. from main.\n");
		return 1;
	}

	// Listening for connection:
	printf("Listening for connections...\n");
	if ( !listenAddress(sockfd) ) {
		printf("Couldn't listen for connections.\n");
		return 1;
	}
	
	// Accept connection:
	printf("Accepting a connection.\n");
	if ( !acceptConn(sockfd, &new_fd) ) {
		printf("Couldn't accept connection.\n");
		return 1;
	}

	// Receiving data:
	printf("Receiving data from connection.\n");
	if ( !receiveData(new_fd, buf) ) {
		printf("Couldn't receive data from connection.\n");
		return 1;
	} else {
		printf("Received data: \n %s", buf);
	}
    return 0;
}
#endif
