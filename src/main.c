#include "addr.h"
#include <stdio.h>
#include <arpa/inet.h>

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
    char* address = "1.2.2.2";
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
#endif