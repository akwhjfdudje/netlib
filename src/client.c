#include "client.h"
#include "addr.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int net_connect(const char* ip, const char* port, const struct addrinfo* config, int *out_sockfd) {
    struct addrinfo *res, *r;
    int sockfd;

    if (!net_get_addresses(ip, port, config, &res)) {
        return 0;
    }

    for (r = res; r != NULL; r = r->ai_next) {
        if ((sockfd = socket(r->ai_family, r->ai_socktype, r->ai_protocol)) == -1) {
            continue;
        }

        if (connect(sockfd, r->ai_addr, r->ai_addrlen) == -1) {
            close(sockfd);
            continue;
        }

        break;
    }

    freeaddrinfo(res);

    if (r == NULL) {
        return 0;
    }

    *out_sockfd = sockfd;
    return 1;
}

int net_send_all(int sockfd, const char* msg, size_t len) {
    size_t total = 0;
    while (total < len) {
        ssize_t bytes = send(sockfd, msg + total, len - total, 0);
        if (bytes == -1) {
            return 0;
        }
        total += bytes;
    }
    return 1;
}

int net_start_client(const char *ip, const char *port) {
    struct addrinfo config;
    int sockfd;

    if (!net_create_config(&config)) {
        return -1;
    }

    if (!net_connect(ip, port, &config, &sockfd)) {
        return -1;
    }

    return sockfd;
}
