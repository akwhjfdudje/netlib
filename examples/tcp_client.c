#include "../include/netlib.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    int sock_fd = net_start_client("127.0.0.1", "8080");
    if (sock_fd == -1) {
        fprintf(stderr, "Failed to connect to server\n");
        return 1;
    }

    char *msg;
    size_t len;
    if (net_recv_msg(sock_fd, &msg, &len)) {
        printf("Received from server: %s\n", msg);
        free(msg);
    }

    close(sock_fd);
    return 0;
}
