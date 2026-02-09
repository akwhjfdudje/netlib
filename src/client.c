#include "client.h"
#include "server.h"
#include "addr.h"
#include "log.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/tcp.h>

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

int net_set_keepalive(int sockfd) {
    int optval = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_KEEPALIVE, &optval, sizeof(optval)) < 0) {
        return 0;
    }
    return 1;
}

int net_start_client(const char *ip, const char *port) {
    struct addrinfo config;
    int sockfd;

    if (!net_create_config(&config)) {
        NET_LOG_E("Failed to create client config.");
        return -1;
    }

    int retries = 3;
    while (retries > 0) {
        if (net_connect(ip, port, &config, &sockfd)) {
            NET_LOG_I("Connected to %s:%s", ip, port);
            return sockfd;
        }
        retries--;
        if (retries > 0) {
            NET_LOG_W("Connection failed, retrying in 1s... (%d retries left)", retries);
            sleep(1);
        }
    }

    NET_LOG_E("Failed to connect to %s:%s after retries.", ip, port);
    return -1;
}

int net_start_client_timeout(const char *ip, const char *port, int timeout_seconds) {
    int sockfd = net_start_client(ip, port);
    if (sockfd == -1) return -1;

    if (!net_set_timeout(sockfd, timeout_seconds)) {
        close(sockfd);
        return -1;
    }

    return sockfd;
}
