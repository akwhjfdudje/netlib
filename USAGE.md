# netlib Usage Examples

This guide provides practical examples for common networking tasks using `netlib`. With the unified `netlib.h` header, you can access all features easily.

## Simple TCP Messaging (Client/Server)

This is the recommended way to send data to ensure it is framed correctly using length-prefixing.

### Server Implementation
```c
#include "netlib.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

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
#include "netlib.h"
#include <unistd.h>

int main() {
    // Connect to local server (with automatic retries)
    int sock_fd = net_start_client("127.0.0.1", "9101");
    
    // Easy string sending
    net_send_str(sock_fd, "Hello from netlib client!");
    
    close(sock_fd);
    return 0;
}
```

## UDP Communication

### Simple UDP Send/Receive
```c
#include "netlib.h"
#include <unistd.h>

// Sender
void send_probe() {
    int fd = net_udp_start(NULL, NULL);
    net_udp_sendto(fd, "127.0.0.1", "9104", "Probe", 5);
    close(fd);
}

// Receiver
void start_receiver() {
    int fd = net_udp_start(NULL, "9104");
    char buf[1024];
    int n = net_udp_recvfrom(fd, buf, 1024);
    close(fd);
}
```

### UDP Broadcast
```c
#include "netlib.h"
#include <unistd.h>

int main() {
    int fd = net_udp_start(NULL, NULL);
    net_udp_enable_broadcast(fd);
    
    net_udp_sendto(fd, "255.255.255.255", "9104", "Discovery", 9);
    
    close(fd);
    return 0;
}
```

## Setting Timeouts

Prevent your application from hanging on dead connections.

```c
#include "netlib.h"

void handle_client(int client_fd) {
    // Set a 5-second timeout for both send and receive
    net_set_timeout(client_fd, 5);
    
    char buf[1024];
    int bytes = net_recv_all(client_fd, buf, 1024);
}

// Or start a server with a default timeout and custom backlog
int start_secure_server() {
    return net_start_server_ext(NULL, "8080", 50, 10); // 50 backlog, 10s timeout
}
```
