#ifndef NETLIB_SERVER_H
#define NETLIB_SERVER_H

#include "addr.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>

/**
 * @brief Sets send and receive timeouts for a socket.
 * 
 * @param sockfd Socket file descriptor.
 * @param seconds Timeout in seconds.
 * @return 1 on success, 0 on failure.
 */
int net_set_timeout(int sockfd, int seconds);

/**
 * @brief Binds a socket to a specific IP and port.
 * 
 * @param ip IP address string (can be NULL for any).
 * @param port Port number string.
 * @param config Addrinfo configuration.
 * @param out_sockfd Pointer to store the resulting socket file descriptor.
 * @return 1 on success, 0 on failure.
 */
int net_bind(const char* ip, const char* port, const struct addrinfo* config, int *out_sockfd);

/**
 * @brief Sets a socket to listening mode.
 * 
 * @param sockfd Socket file descriptor.
 * @param backlog Maximum length of the queue of pending connections.
 * @return 1 on success, 0 on failure.
 */
int net_listen(int sockfd, int backlog);

/**
 * @brief Accepts a new connection on a listening socket.
 * 
 * @param sockfd Listening socket file descriptor.
 * @param out_client_fd Pointer to store the new client socket file descriptor.
 * @return 1 on success, 0 on failure.
 */
int net_accept(int sockfd, int *out_client_fd);

/**
 * @brief Receives exactly `len` bytes from a socket.
 * 
 * @param sockfd Socket file descriptor.
 * @param buf Buffer to store received data.
 * @param len Number of bytes to receive.
 * @return Number of bytes received on success, -1 on failure/closure.
 */
int net_recv_all(int sockfd, char *buf, size_t len);

/**
 * @brief Helper to start a server (bind + listen).
 * 
 * @param ip IP address string.
 * @param port Port number string.
 * @return Listening socket file descriptor on success, -1 on failure.
 */
int net_start_server(const char *ip, const char *port);

#endif
