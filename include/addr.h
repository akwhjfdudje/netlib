#ifndef NETLIB_ADDR_H
#define NETLIB_ADDR_H

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

/**
 * @brief Converts an IP address string to a binary in_addr representation.
 * 
 * @param version 0 for IPv4, 1 for IPv6.
 * @param address The IP address string.
 * @param out_addr Pointer to the in_addr struct to store the result.
 * @return 1 on success, 0 on failure.
 */
int net_addr_to_bin(int version, const char* address, struct in_addr* out_addr);

/**
 * @brief Converts a binary in_addr representation to an IP address string.
 * 
 * @param version 0 for IPv4, 1 for IPv6.
 * @param in_addr Pointer to the in_addr struct.
 * @param out_ip Buffer to store the resulting IP string.
 * @param len Length of the out_ip buffer.
 * @return 1 on success, 0 on failure.
 */
int net_bin_to_addr(int version, struct in_addr* in_addr, char* out_ip, socklen_t len);

/**
 * @brief Initializes an addrinfo configuration with default values.
 * 
 * @param config Pointer to the addrinfo struct to initialize.
 * @return 1 on success, 0 on failure.
 */
int net_create_config(struct addrinfo* config);

/**
 * @brief Performs DNS resolution to get a list of addresses.
 * 
 * @param ip The hostname or IP string.
 * @param port The port number or service name string.
 * @param config Pointer to the addrinfo configuration.
 * @param res Pointer to store the resulting linked list of addresses.
 * @return 1 on success, 0 on failure.
 */
int net_get_addresses(const char* ip, const char* port, const struct addrinfo* config, struct addrinfo **res);

#endif
