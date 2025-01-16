#include "addr.h"
#include "client.h"
#include "server.h"
#include "stdlib.h"
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <limits.h>
#include <unistd.h>


// https://stackoverflow.com/questions/3747086/reading-the-whole-text-file-into-a-char-array-in-c 
char* load_file(char const* path)
{
    char* buffer = 0;
    long length;
    FILE* f = fopen (path, "rb"); 

    if (f) {
		fseek (f, 0, SEEK_END);
		length = ftell (f);
		fseek (f, 0, SEEK_SET);
		buffer = (char*)malloc((length+1)*sizeof(char));
		if (buffer) {
			fread(buffer, sizeof(char), length, f);
		}
		fclose (f);
    }
    buffer[length] = '\0';
	return buffer;
}

// Testing code
int main(int argc, char **argv) {

	// Test for checking IP conversion
	if ( argc == 2 && strcmp(argv[1], "127.0.0.1") == 0 ) {

		// Declaring variables for first test:
		// address: stores address string to test conversion
		// v: stores ip version
		// sockaddr: stores sockaddr_in struct
		char* address = argv[1];
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
		return 0;
	}

	// Declaring variables:
	// sockfd: stores the file descriptor for the binding
	// new_fd: stores the file descriptor for sending and receiving
	// config: stores the configuration for the binding
	int sockfd, new_fd;
	struct addrinfo config;
	char buf[LINE_MAX];
	char* address = "127.0.0.1"; 
	
	// Test to bind and listen on an address:
	if ( argc == 2 && strcmp(argv[1], "serve") == 0 ) {
	
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

		return 0;
	}

	// Test to connect to an address:
	if ( argc == 2 && strcmp(argv[1], "connect") == 0 ) {

		// Create the config
		if ( !createConfig(&config) ) {
			printf("Couldn't create config.\n");
			return 1;
		}

		// Connecting to server:
		printf("Connecting to server:\n");
		if ( !sendServer(address, "8080", &config, &sockfd, "client test\n") ) {
			printf("Couldn't connect to server.\n");
			return 1;
		}

		return 0;
	}
	
	// TODO: make these tests
	// Test to send a large amount of data
	// Test to send a file
	if ( argc == 2 && strcmp(argv[1], "sendfile") == 0 ) {

		// Opening the file:
		char* src = load_file("/etc/passwd");

		// Create the config
		if ( !createConfig(&config) ) {
			printf("Couldn't create config.\n");
			return 1;
		}

		// Sending the file:
		if ( !sendServer(address, "8080", &config, &sockfd, src) ) {
			printf("Couldn't send file.\n");
			free(src);
			return 1;
		}

		// Free the buffer at the end:
		free(src);
		return 0;
	}
	// Test to receive a large amount of data
	// Test to receive a file
    return 0;
}
