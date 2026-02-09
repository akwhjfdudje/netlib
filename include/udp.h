#ifndef NETLIB_UDP_H
#define NETLIB_UDP_H

#include "addr.h"
#include <sys/types.h>
#include <sys/socket.h>

/**
 * @brief Starts a UDP socket (binds if port is provided).
 * 
 * @param ip IP address string (can be NULL).
 * @param port Port number string (can be NULL).
 * @return Socket file descriptor on success, -1 on failure.
 */
int net_udp_start(const char *ip, const char *port);

/**
 * @brief Sends a UDP packet to a specific address.
 * 
 * @param sockfd UDP socket file descriptor.
 * @param ip Destination IP or hostname.
 * @param port Destination port.
 * @param msg Data to send.
 * @param len Length of data.
 * @return 1 on success, 0 on failure.
 */
int net_udp_sendto(int sockfd, const char *ip, const char *port, const char *msg, size_t len);

/**
 * @brief Receives a UDP packet.
 * 
 * @param sockfd UDP socket file descriptor.
 * @param buf Buffer to store data.
 * @param len Maximum length to receive.
 * @return Number of bytes received on success, -1 on failure.
 */
int net_udp_recvfrom(int sockfd, char *buf, size_t len);

#endif
