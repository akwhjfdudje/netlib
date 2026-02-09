# netlib API Documentation

This document provides a detailed overview of the modules and functions available in `netlib`.

## Modules

- [Address Management (`addr.h`)](#address-management-addrh)
- [TCP Client (`client.h`)](#tcp-client-clienth)
- [TCP Server (`server.h`)](#tcp-server-serverh)
- [Messaging Protocol (`msg.h`)](#messaging-protocol-msgh)
- [UDP Communication (`udp.h`)](#udp-communication-udph)
- [Logging System (`log.h`)](#logging-system-logh)

---

## Address Management (`addr.h`)

Functions for resolving hostnames and converting IP addresses.

### `net_addr_to_bin`
```c
int net_addr_to_bin(int version, const char* address, struct in_addr* out_addr);
```
Converts an IP string (v4 or v6) to binary format. `version` is 0 for IPv4, 1 for IPv6.

### `net_bin_to_addr`
```c
int net_bin_to_addr(int version, struct in_addr* in_addr, char* out_ip, socklen_t len);
```
Converts binary IP to a human-readable string.

---

## TCP Client (`client.h`)

### `net_start_client`
```c
int net_start_client(const char *ip, const char *port);
```
Connects to a remote server. Includes 3 automatic retries with 1s delays. Returns socket FD or -1.

### `net_set_keepalive`
```c
int net_set_keepalive(int sockfd);
```
Enables TCP Keep-Alive on the socket.

---

## TCP Server (`server.h`)

### `net_start_server`
```c
int net_start_server(const char *ip, const char *port);
```
Binds and listens on the specified port. Returns listening socket FD or -1.

### `net_accept`
```c
int net_accept(int sockfd, int *out_client_fd);
```
Accepts a pending connection.

### `net_set_timeout`
```c
int net_set_timeout(int sockfd, int seconds);
```
Sets both Send and Receive timeouts for the socket.

---

## Messaging Protocol (`msg.h`)

Provides automatic data framing using a 4-byte length prefix.

### `net_send_msg`
```c
int net_send_msg(int sockfd, const char *msg, size_t len);
```
Sends a length-prefixed buffer.

### `net_recv_msg`
```c
int net_recv_msg(int sockfd, char **out_buf, size_t *out_len);
```
Allocates a buffer and receives a full message. **Caller must free `out_buf`.**

---

## UDP Communication (`udp.h`)

### `net_udp_start`
```c
int net_udp_start(const char *ip, const char *port);
```
Starts a UDP socket. Bind is performed if `port` is provided.

### `net_udp_sendto` / `net_udp_recvfrom`
Standard UDP Send/Receive helpers.

---

## Logging System (`log.h`)

### `net_log_set_level`
```c
void net_log_set_level(net_log_level_t level);
```
Sets verbosity: `NET_LOG_NONE`, `ERROR`, `WARN`, `INFO`, `DEBUG`.

### Logging Macros
- `NET_LOG_I(...)` - Info
- `NET_LOG_W(...)` - Warning
- `NET_LOG_E(...)` - Error
- `NET_LOG_D(...)` - Debug
