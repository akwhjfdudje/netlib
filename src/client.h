#ifndef CLIENT_H
#define CLIENT_H
#include "addr.h"
#include <sys/types.h>
#include <sys/socket.h>

// Code for handling clients 
int connectAddress(const char* ip, const char* port, const struct addrinfo* config, int *socketf);
int sendData(int sockfd, char* msg, long len);
int sendServer(char *address, char *port, struct addrinfo *config, int *sockfd, char *data, long length);
#endif
