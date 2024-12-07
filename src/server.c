#include "server.h"
#include "addr.h"
#include <stdio.h>

// Function to bind to a given address and port:
// ip, port: contains the ip and port to bind to
// config: contains configuration details for address
int bindAddress(const char* ip, const char* port, const struct addrinfo* config) {

    // Declaring variables:
    // r: stores the pointer to loop with
    // res: stores the results of querying for addresses
    struct addrinfo *r;
    struct addrinfo **res;

    // Getting address results
    if ( !(getAddresses(ip, port, config, res)) ) {
        printf("Couldn't get address results.\n")
        return 0;
    }

    // Looping through every element to get the first valid result:
    for ( r = res; r != NULL; r = r->ai_next ) {
        
        // Binding addresses
    }
}