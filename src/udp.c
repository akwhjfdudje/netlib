#include "udp.h"
#include "addr.h"
#include <netdb.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>

int net_udp_start(const char *ip, const char *port) {
    struct addrinfo config, *res, *r;
    int sockfd;

    memset(&config, 0, sizeof(config));
    config.ai_family = AF_UNSPEC;
    config.ai_socktype = SOCK_DGRAM;
    if (ip == NULL && port != NULL) config.ai_flags = AI_PASSIVE;

    if (!net_get_addresses(ip, port, &config, &res)) {
        return -1;
    }

    for (r = res; r != NULL; r = r->ai_next) {
        if ((sockfd = socket(r->ai_family, r->ai_socktype, r->ai_protocol)) == -1) {
            continue;
        }

        if (ip != NULL || port != NULL) {
            if (bind(sockfd, r->ai_addr, r->ai_addrlen) == -1) {
                close(sockfd);
                continue;
            }
        }
        break;
    }

    freeaddrinfo(res);
    if (r == NULL) return -1;

    return sockfd;
}

int net_udp_sendto(int sockfd, const char *ip, const char *port, const char *msg, size_t len) {
    struct addrinfo config, *res;
    memset(&config, 0, sizeof(config));
    config.ai_family = AF_UNSPEC;
    config.ai_socktype = SOCK_DGRAM;

    if (!net_get_addresses(ip, port, &config, &res)) {
        return 0;
    }

    ssize_t bytes = sendto(sockfd, msg, len, 0, res->ai_addr, res->ai_addrlen);
    freeaddrinfo(res);

    return (bytes != -1);
}

int net_udp_recvfrom(int sockfd, char *buf, size_t len) {
    struct sockaddr_storage addr;
    socklen_t addrlen = sizeof(addr);
    ssize_t bytes = recvfrom(sockfd, buf, len, 0, (struct sockaddr *)&addr, &addrlen);
    return (int)bytes;
}

int net_udp_enable_broadcast(int sockfd) {
    int optval = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &optval, sizeof(optval)) < 0) {
        return 0;
    }
    return 1;
}
