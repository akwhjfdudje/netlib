#ifndef SERV_H
#define SERV_H
#include "addr.h"
#include <sys/types.h>
#include <sys/socket.h>

// Code for handling servers
int bindAddress(const char* ip, const char* port, const struct addrinfo* config, int *socketf);
int listenAddress(int sockfd);
int acceptConn(int sockfd, int *new_fd);
int receiveData(int sockfd, char *buf);
int startServer(struct addrinfo *config, char *address, char *port);
#endif
