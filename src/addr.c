#include "addr.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>

// Function to get useable form of given ip:
// v: stores the ip version to use: 0 for ipv4, 1 for ipv6
// address: stores a pointer to char that stores the ip address
// sockaddr: stores the output from inet_pton
int getIPToInteger(int v, const char* address, struct in_addr* sockaddr) {
    
    // Declaring variables:
    // status: to store result of inet_pton
    // ip_f: stores the address family to use
    int status, ip_f;

    // Getting value for ip_f:
    if ( v == 0 ) {
        ip_f = AF_INET;
    } else if ( v == 1 ) {
        ip_f = AF_INET6;
    } else {
        printf("Invalid value for v.\n");
        return 0;
    }

    // Checking if function was successful:
    if ( (status = inet_pton(ip_f, address, sockaddr)) == 0 ) {
        printf("Invalid address string.\n");
        return 0;
    }

    if ( status == -1 ) {
        printf("ip_f: %d\n", ip_f);
        printf("Invalid address family.\n");
        return 0;
    }

    return 1;
}

// Function to get ip from useable form:
// v: stores the ip version to use
// sockaddr: in_addr struct to convert from
// ip: stores the ip address from conversion
// len: length of ip buffer
int getIntegerToIP(int v, struct in_addr* sockaddr, char* ip, socklen_t len) {

    // Declaring variables:
    // ip_f: stores the address family to use
    // ip_st: stores pointer result from inet_ntop
    int ip_f;
    const char* ip_st;

    // Getting value for ip_f:
    if ( v == 0 ) {
        ip_f = AF_INET;
    } 
    
    if ( v == 1 ) {
        ip_f = AF_INET6;
    } 
    
    if ( v == -1 ) {
        ip_f = AF_UNSPEC;
    }

    if ( (ip_st = inet_ntop(ip_f, sockaddr, ip, len)) == NULL ) { 
        printf("Can't get pointer to ip address.\n");
        return 0;
    }

    return 1;
}

// Function that gets address results from query:
// ip: stores the ip or hostname
// port: stores the port number in string format
// config: stores the instructions for what kind of results to get
// res: contains pointer to linked list of results
int getAddresses(const char* ip, const char* port, const struct addrinfo* config, struct addrinfo **res) {

    // Declaring status variable to store result of getaddrinfo
    int status;

    if ( (status = getaddrinfo(ip, port, config, res)) != 0 ) {
        printf("Couldn't get results: %s\n", gai_strerror(status));
        return 0;
    }

    return 1;
}