#include "addr.h"
#include "client.h"
#include "server.h"
#include "stdlib.h"
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <limits.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>


// https://stackoverflow.com/questions/3747086/reading-the-whole-text-file-into-a-char-array-in-c 
char* load_file(char const* path, long* len) {
    char* buffer = 0;
    long length;
    FILE* f = fopen (path, "rb"); 

    if (f) {
		fseek (f, 0, SEEK_END);
		length = ftell (f);
		printf("length of file: %ld\n", length);
		fseek (f, 0, SEEK_SET);
		buffer = (char*)malloc((length+1)*sizeof(char));
		if (buffer) {
			fread(buffer, sizeof(char), length, f);
		}
		fclose (f);
    }
	printf("length of file: %ld\n", length);
    buffer[length] = '\0';
	printf("second last char: %c\n", buffer[length - 1247]);
	*len = length;
	return buffer;
}

// Function to load random bytes into a buffer
char* load_random(long len) {

	// Declaring variables:
	int rand;
	ssize_t r;

	// Open random stream
	if ( (rand = open("/dev/urandom", O_RDONLY)) < 0 ) {
		perror("open");
		printf("Couldn't open file.\n");
		return NULL;
	}

	// Create buffer, read bytes into the buffer
	char* buffer = malloc(len * sizeof(char));
	if ( (r = read(rand, buffer, len)) < 0) {
		perror("read");
		printf("Couldn't read the file.\n");
		return NULL;
	}

	// Replace all null characters in the buffer:
	for ( int i = 0; i < len; i++ ) {
		if ( buffer[i] == '\0' ) {
			buffer[i] = 'a';
		}
	}

	// Return the pointer
	return buffer;	
	
}

// Testing code
int main(int argc, char **argv) {

	// Test for checking IP conversion
	if ( argc == 2 && strcmp(argv[1], "convert") == 0 ) {

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

		char* test_text = "client test\n";

		// Create the config
		if ( !createConfig(&config) ) {
			printf("Couldn't create config.\n");
			return 1;
		}

		// Connecting to server:
		printf("Connecting to server:\n");
		if ( !sendServer(address, "8080", &config, &sockfd, test_text, strlen(test_text)) ) {
			printf("Couldn't connect to server.\n");
			return 1;
		}

		return 0;
	}
	
	// TODO: make these tests
	// Test to send a large amount of data
	if ( argc == 3 && strcmp(argv[1], "sendlarge") == 0 ) {
		
		// Declaring variables:
		long length = atoi(argv[2]);	
		char* buffer = load_random(length);
		printf("length: %ld\n", length);

		// Checking buffer:
		if ( buffer == NULL ) {
			printf("Couldn't load bytes.\n");
			return 1;
		}

		// Creating config:
		if ( !createConfig(&config) ) {
			printf("Couldn't create config.\n");
			return 1;
		}

		// Sending data:
		if ( !sendServer(address, "8080", &config, &sockfd, buffer, length) ) {
			printf("Couldn't send data.\n");
			free(buffer);	
			return 1;
		}

		// Exiting:
		free(buffer);
		return 0;
	}

	// Test to send a file
	if ( argc == 2 && strcmp(argv[1], "sendfile") == 0 ) {

		// Opening the file:
		long len;
		char* src = load_file("/etc/passwd", &len);

		// Create the config
		if ( !createConfig(&config) ) {
			printf("Couldn't create config.\n");
			return 1;
		}

		// Sending the file:
		if ( !sendServer(address, "8080", &config, &sockfd, src, len) ) {
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
