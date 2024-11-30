#ifndef ADDR_H
#define ADDR_H
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
// Code for handling addresses

int getIPToInteger(int v, const char* address, struct in_addr*);
int getIntegerToIP(int v, struct in_addr*, char*, len);
int createAddress(const char* ip, const char* service, const struct addrinfo* hints, struct addrinfo **res);

#endif