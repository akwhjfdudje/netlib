#include "addr.h"
#include "client.h"
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
    
	// TODO : add client tests
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

	// Starting server:
	printf("Starting server:\n");
	if ( (new_fd = startServer(&config, address, "9001")) == -1 ) {
		printf("Couldn't start server.\n");
		return 1;
	}
	
	// Receiving data:
	printf("Receiving data from connection.\n");
	if ( !receiveData(new_fd, buf) ) {
		printf("Couldn't receive data from connection.\n");
		return 1;
	} else {
		printf("Received data: \n%s", buf);
	}

	// Test to connect to an address:

	// Connecting to server:
	printf("Connecting to server:\n");
	if ( !sendServer(address, "8080", &config, &sockfd, "Hello again!\n") ) {
		printf("Couldn't connect to server.\n");
		return 1;
	}
	
    return 0;
}
#endif
