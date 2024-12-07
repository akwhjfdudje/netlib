#ifndef SERV_H
#define SERV_H
#include <sys/types.h>
#include <sys/socket.h>

// Code for handling servers
// TODO: Create server structs and stuff here
int bindAddress(const char* ip, const char* port, const struct addrinfo* config);
#endif
