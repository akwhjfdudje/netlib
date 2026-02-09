#ifndef NETLIB_CLIENT_H
#define NETLIB_CLIENT_H

#include "addr.h"
#include <sys/types.h>
#include <sys/socket.h>

/**
 * @brief Enables TCP Keep-Alive on a socket.
 * 
 * @param sockfd Socket file descriptor.
 * @return 1 on success, 0 on failure.
 */
int net_set_keepalive(int sockfd);

/**
 * @brief Connects a socket to a remote IP and port.
 * 
 * @param ip Remote IP address or hostname.
 * @param port Remote port number or service name.
 * @param config Addrinfo configuration.
 * @param out_sockfd Pointer to store the resulting socket file descriptor.
 * @return 1 on success, 0 on failure.
 */
int net_connect(const char* ip, const char* port, const struct addrinfo* config, int *out_sockfd);

/**
 * @brief Sends exactly `len` bytes over a socket.
 * 
 * @param sockfd Socket file descriptor.
 * @param msg Buffer containing data to send.
 * @param len Number of bytes to send.
 * @return 1 on success, 0 on failure.
 */
int net_send_all(int sockfd, const char* msg, size_t len);

/**
 * @brief Helper to connect to a server.
 * 
 * @param ip Remote IP or hostname.
 * @param port Remote port.
 * @return Connected socket file descriptor on success, -1 on failure.
 */
int net_start_client(const char *ip, const char *port);

#endif
