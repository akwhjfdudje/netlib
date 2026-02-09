# netlib

A simple C networking library for TCP and UDP communication.

## Quick Start

To build the static library (`libnet.a`) and run the test suite:

```bash
make
./test
```

## Features

- **TCP**: Guaranteed data transfer and automatic framing.
- **UDP**: Simple messaging and broadcast support.
- **Logging**: Structured logging and clean API.

## Documentation

- [**API Reference**](DOCUMENTATION.md): Detailed function descriptions.
- [**Usage Guide**](USAGE.md): Practical code examples.

## Quick Example (TCP)

```c
#include "netlib.h"

int main() {
    // Start a server on port 8080
    int server_fd = net_start_server(NULL, "8080");
    
    // In another process: Connect to it
    int client_fd = net_start_client("127.0.0.1", "8080");
    
    // Send a length-prefixed string
    net_send_str(client_fd, "Hello netlib!");
    
    return 0;
}
```

## Integration

Include the headers from `include/` and link against `libnet.a`:

```bash
gcc main.c -Iinclude -L. -lnet -o my_app
```
