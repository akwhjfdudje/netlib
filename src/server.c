#include "server.h"
#include "addr.h"
#include <netdb.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>

int net_set_timeout(int sockfd, int seconds) {
    struct timeval timeout;
    timeout.tv_sec = seconds;
    timeout.tv_usec = 0;

    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
        return 0;
    }
    if (setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout)) < 0) {
        return 0;
    }
    return 1;
}

int net_bind(const char* ip, const char* port, const struct addrinfo* config, int *out_sockfd) {
    struct addrinfo *res, *r;
    int sockfd;
    int yes = 1;

    if (!net_get_addresses(ip, port, config, &res)) {
        return 0;
    }

    for (r = res; r != NULL; r = r->ai_next) {
        if ((sockfd = socket(r->ai_family, r->ai_socktype, r->ai_protocol)) == -1) {
            continue;
        }

        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
            close(sockfd);
            continue;
        }

        if (bind(sockfd, r->ai_addr, r->ai_addrlen) == -1) {
            close(sockfd);
            continue;
        }

        break;
    }

    freeaddrinfo(res);

    if (r == NULL) {
        perror("net_bind");
        return 0;
    }

    *out_sockfd = sockfd;
    return 1;
}

int net_listen(int sockfd, int backlog) {
    if (listen(sockfd, backlog) == -1) {
        return 0;
    }
    return 1;
}

int net_accept(int sockfd, int *out_client_fd) {
    struct sockaddr_storage addr;
    socklen_t addrlen = sizeof(addr);
    int client_fd;

    if ((client_fd = accept(sockfd, (struct sockaddr *)&addr, &addrlen)) == -1) {
        return 0;
    }

    *out_client_fd = client_fd;
    return 1;
}

int net_recv_all(int sockfd, char *buf, size_t len) {
    size_t total = 0;
    while (total < len) {
        ssize_t bytes = recv(sockfd, buf + total, len - total, 0);
        if (bytes <= 0) {
            return -1;
        }
        total += bytes;
    }
    return (int)total;
}

int net_start_server(const char *ip, const char *port) {
    struct addrinfo config;
    int sockfd;

    if (!net_create_config(&config)) {
        return -1;
    }
    config.ai_flags |= AI_PASSIVE;

    if (!net_bind(ip, port, &config, &sockfd)) {
        return -1;
    }

    if (!net_listen(sockfd, 10)) {
        close(sockfd);
        return -1;
    }

    return sockfd;
}
