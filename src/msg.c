#include "msg.h"
#include "client.h"
#include "server.h"
#include "log.h"
#include <stdlib.h>
#include <stdint.h>
#include <arpa/inet.h>

int net_send_msg(int sockfd, const char *msg, size_t len) {
    uint32_t net_len = htonl((uint32_t)len);
    if (!net_send_all(sockfd, (const char *)&net_len, sizeof(net_len))) {
        NET_LOG_E("Failed to send message length prefix.");
        return 0;
    }
    if (!net_send_all(sockfd, msg, len)) {
        NET_LOG_E("Failed to send message body.");
        return 0;
    }
    return 1;
}

int net_recv_msg(int sockfd, char **out_buf, size_t *out_len) {
    uint32_t net_len;
    if (net_recv_all(sockfd, (char *)&net_len, sizeof(net_len)) <= 0) {
        NET_LOG_E("Failed to receive message length prefix.");
        return 0;
    }
    
    uint32_t len = ntohl(net_len);
    char *buf = malloc(len + 1);
    if (buf == NULL) {
        NET_LOG_E("Failed to allocate buffer for received message (size %u).", len);
        return 0;
    }
    
    if (net_recv_all(sockfd, buf, len) <= 0) {
        NET_LOG_E("Failed to receive message body.");
        free(buf);
        return 0;
    }
    
    buf[len] = '\0';
    *out_buf = buf;
    *out_len = (size_t)len;
    return 1;
}
