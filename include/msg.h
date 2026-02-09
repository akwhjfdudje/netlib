#ifndef NETLIB_MSG_H
#define NETLIB_MSG_H

#include <stddef.h>

/**
 * @brief Sends a length-prefixed message over a TCP socket.
 * 
 * @param sockfd Socket file descriptor.
 * @param msg Buffer containing the message.
 * @param len Length of the message.
 * @return 1 on success, 0 on failure.
 */
int net_send_msg(int sockfd, const char *msg, size_t len);

/**
 * @brief Receives a length-prefixed message from a TCP socket.
 * 
 * @param sockfd Socket file descriptor.
 * @param out_buf Pointer to a buffer that will be allocated (must be freed by caller).
 * @param out_len Pointer to store the length of the received message.
 * @return 1 on success, 0 on failure.
 */
int net_recv_msg(int sockfd, char **out_buf, size_t *out_len);

#endif
