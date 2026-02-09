#include "addr.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

int net_addr_to_bin(int version, const char* address, struct in_addr* out_addr) {
    int family;
    if (version == 0) {
        family = AF_INET;
    } else if (version == 1) {
        family = AF_INET6;
    } else {
        return 0;
    }

    int status = inet_pton(family, address, out_addr);
    if (status <= 0) {
        return 0;
    }

    return 1;
}

int net_bin_to_addr(int version, struct in_addr* in_addr, char* out_ip, socklen_t len) {
    int family;
    if (version == 0) {
        family = AF_INET;
    } else if (version == 1) {
        family = AF_INET6;
    } else {
        return 0;
    }

    if (inet_ntop(family, in_addr, out_ip, len) == NULL) { 
        return 0;
    }

    return 1;
}

int net_create_config(struct addrinfo* config) {
	if (config == NULL) {
		return 0;
	}
	
	memset(config, 0, sizeof(struct addrinfo));
	config->ai_family = AF_UNSPEC;
	config->ai_socktype = SOCK_STREAM;
	config->ai_flags = (AI_V4MAPPED | AI_ADDRCONFIG);
	return 1;
}

int net_get_addresses(const char* ip, const char* port, const struct addrinfo* config, struct addrinfo **res) {
    if (getaddrinfo(ip, port, config, res) != 0) {
        return 0;
    }
    return 1;
}
