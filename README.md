# netlib

A simple, robust C networking library for TCP and UDP communication.

## Features

- **TCP Client/Server**: Robust data transfer with `net_send_all` and `net_recv_all`.
- **UDP Support**: Connectionless messaging with `net_udp_sendto` and `net_udp_recvfrom`.
- **Address Abstraction**: Easy IP string to binary conversion and DNS resolution.
- **Socket Timeouts**: Prevent blocking indefinitely with `net_set_timeout`.
- **Self-Contained Tests**: Comprehensive C-based test suite.

## Building

To build the static library (`libnet.a`) and the test binary:

```bash
make
```

## Usage

### TCP Server Example

```c
#include "server.h"
#include <unistd.h>

int main() {
    int listen_fd = net_start_server(NULL, "9001");
    int client_fd;
    if (net_accept(listen_fd, &client_fd)) {
        char buf[1024];
        int bytes = net_recv_all(client_fd, buf, 1024);
        // Process data...
        close(client_fd);
    }
    close(listen_fd);
    return 0;
}
```

### TCP Client Example

```c
#include "client.h"
#include <string.h>
#include <unistd.h>

int main() {
    int sock_fd = net_start_client("127.0.0.1", "9001");
    const char *msg = "Hello!";
    net_send_all(sock_fd, msg, strlen(msg));
    close(sock_fd);
    return 0;
}
```

### UDP Example

```c
#include "udp.h"
#include <unistd.h>

int main() {
    // Receive
    int sockfd = net_udp_start(NULL, "9004");
    char buf[64];
    net_udp_recvfrom(sockfd, buf, 64);
    close(sockfd);
    
    // Send
    int send_fd = net_udp_start(NULL, NULL);
    net_udp_sendto(send_fd, "127.0.0.1", "9004", "Hi", 2);
    close(send_fd);
    return 0;
}
```

## Running Tests

```bash
./test
```

## API Overview

### `addr.h`
- `net_addr_to_bin`: IP string -> `in_addr`.
- `net_bin_to_addr`: `in_addr` -> IP string.
- `net_get_addresses`: DNS resolution helper.

### `server.h` / `client.h`
- `net_start_server` / `net_start_client`: High-level startup helpers.
- `net_send_all` / `net_recv_all`: Guaranteed full data transfer.
- `net_set_timeout`: Set RCV/SND timeouts.

### `udp.h`
- `net_udp_start`: Initialize UDP socket.
- `net_udp_sendto` / `net_udp_recvfrom`: Basic UDP I/O.
