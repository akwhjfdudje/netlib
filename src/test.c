#include "addr.h"
#include "client.h"
#include "server.h"
#include "stdlib.h"
#include <bits/posix2_lim.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
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

	int openResult;
	ssize_t readResult;

	if ( (openResult = open("/dev/urandom", O_RDONLY)) < 0 ) {
		perror("open");
		printf("Couldn't open file.\n");
		return NULL;
	}

	char* buffer = malloc(len * sizeof(char));
	if ( (readResult = read(openResult, buffer, len)) < 0) {
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

		// Test for getIPToInteger 
		printf("'getIPToInteger' test result: %d\n", getIPToInteger(v, address, &(sockaddr.sin_addr)));
		
		// Declaring variables for second test:
		// ip: will store ip from function
		char ip[INET_ADDRSTRLEN];

		// Test for getIntegerToIP
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
	char* address = "127.0.0.1"; 
	
	// Test to bind and listen on an address:
	if ( argc == 2 && strcmp(argv[1], "serve") == 0 ) {
	
		char buf[100] = {};
		int bytes;
		FILE* f = fopen("./test.file", "w+");

		// Starting server:
		printf("Starting server:\n");
		if ( (new_fd = startServer(&config, address, "9001")) == -1 ) {
			printf("Couldn't start server.\n");
			return 1;
		}
		
		// Receiving data:
		printf("Receiving data from connection.\n");
		if ( (bytes = receiveData(new_fd, buf, 100)) == -1 ) {
			printf("Couldn't receive data from connection.\n");
			return 1;
		} else {
			printf("Bytes received: %d\n", bytes);
			printf("Received data: \n%s", buf);
		}

		// Writing to file:
		fwrite(buf, sizeof(char), 100, f);

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
	if ( argc == 3 && strcmp(argv[1], "receivelarge") == 0 ) {

		FILE* f = fopen("./test.file", "w+");
		long bytes;
		int size = atoi(argv[2]); 
		char* randomBuffer = malloc(size * sizeof(char));
		printf("Size from args: %d\n", size);
		printf("Long: %lu", sizeof(long));
		printf("Int: %lu", sizeof(int));
		
		// Starting server:
		if ( (new_fd = startServer(&config, address, "9001")) == -1 ) {
			printf("Couldn't start server.\n");
			free(randomBuffer);
			return 1;
		}
		
		// Receiving data:
		if ( (bytes = receiveData(new_fd, randomBuffer, size)) == -1 ) {
			printf("Couldn't receive data from connection.\n");
			free(randomBuffer);
			return 1;
		} else {
			printf("Bytes received: %ld\n", bytes);
		}

		// Writing to file:
		fwrite(randomBuffer, sizeof(char), size, f);

		free(randomBuffer);
		return 0;
	}

	// Test to receive a file
	if ( argc == 2 && strcmp(argv[1], "receive") == 0 ) {

		// Checking stats of the /etc/passwd file to get the size 
		struct stat st;
		stat("/etc/passwd", &st);
		FILE* f = fopen("./test.file", "w+");
		long size = st.st_size;
		int bytes;
		char* passwdBuffer = malloc(size * sizeof(char));
		printf("Size of passwd: %ld\n", size);
		
		// Starting server:
		if ( (new_fd = startServer(&config, address, "9001")) == -1 ) {
			printf("Couldn't start server.\n");
			free(passwdBuffer);
			return 1;
		}
		
		// Receiving data:
		if ( (bytes = receiveData(new_fd, passwdBuffer, size)) == -1 ) {
			printf("Couldn't receive data from connection.\n");
			free(passwdBuffer);
			return 1;
		} else {
			printf("Received bytes: %d\n", bytes);
		}

		// Writing to file:
		fwrite(passwdBuffer, sizeof(char), size, f);

		free(passwdBuffer);
		return 0;
	}
    return 0;
}
