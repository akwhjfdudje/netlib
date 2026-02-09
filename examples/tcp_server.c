#include "../include/netlib.h"
#include <stdio.h>
#include <unistd.h>

int main() {
    int server_fd = net_start_server(NULL, "8080");
    if (server_fd == -1) return 1;

    printf("Server listening on port 8080...\n");

    while (1) {
        int client_fd;
        if (net_accept(server_fd, &client_fd)) {
            printf("New connection accepted!\n");
            net_send_str(client_fd, "Welcome to netlib server!");
            close(client_fd);
        }
    }

    close(server_fd);
    return 0;
}
