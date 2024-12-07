#ifndef ADDR_H
#define ADDR_H
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
// Code for handling addresses

int getIPToInteger(int v, const char* address, struct in_addr*);
int getIntegerToIP(int v, struct in_addr*, char*, socklen_t len);
//int createConfig(int )
int getAddresses(const char* ip, const char* port, const struct addrinfo* config, struct addrinfo **res);
#endif