# netlib Usage Examples

This guide provides practical examples for common networking tasks using `netlib`.

## Simple TCP Messaging (Client/Server)

This is the recommended way to send data to ensure it is framed correctly.

### Server Implementation
```c
#include "server.h"
#include "msg.h"
#include "log.h"
#include <stdlib.h>

int main() {
    // Start server on port 9101
    int listen_fd = net_start_server(NULL, "9101");
    
    int client_fd;
    if (net_accept(listen_fd, &client_fd)) {
        char *buf;
        size_t len;
        
        // Receive the framed message
        if (net_recv_msg(client_fd, &buf, &len)) {
            printf("Received %zu bytes: %s\n", len, buf);
            free(buf); // Important: free the allocated buffer
        }
        close(client_fd);
    }
    close(listen_fd);
    return 0;
}
```

### Client Implementation
```c
#include "client.h"
#include "msg.h"
#include <string.h>

int main() {
    // Connect to local server (with automatic retries)
    int sock_fd = net_start_client("127.0.0.1", "9101");
    
    const char *data = "Hello from netlib client!";
    net_send_msg(sock_fd, data, strlen(data));
    
    close(sock_fd);
    return 0;
}
```

## UDP Broadcast

Useful for service discovery or local network notifications.

```c
#include "udp.h"
#include <string.h>

int main() {
    int fd = net_udp_start(NULL, NULL);
    net_udp_enable_broadcast(fd);
    
    const char *msg = "Discovery Probe";
    net_udp_sendto(fd, "255.255.255.255", "9104", msg, strlen(msg));
    
    close(fd);
    return 0;
}
```

## Setting Timeouts

Prevent your application from hanging on dead connections.

```c
#include "server.h"

void handle_client(int client_fd) {
    // Set a 5-second timeout for both send and receive
    net_set_timeout(client_fd, 5);
    
    char buf[1024];
    int bytes = net_recv_all(client_fd, buf, 1024);
    if (bytes == -1) {
        // This could be a timeout or a disconnection
    }
}
```
