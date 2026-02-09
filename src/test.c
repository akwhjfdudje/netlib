#include "addr.h"
#include "client.h"
#include "server.h"
#include "udp.h"
#include "msg.h"
#include "log.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>

void test_address_conversion() {
    printf("Testing address conversion... ");
    struct in_addr addr;
    char ip_buf[INET_ADDRSTRLEN];

    assert(net_addr_to_bin(0, "127.0.0.1", &addr) == 1);
    assert(net_bin_to_addr(0, &addr, ip_buf, INET_ADDRSTRLEN) == 1);
    assert(strcmp(ip_buf, "127.0.0.1") == 0);

    assert(net_addr_to_bin(0, "invalid", &addr) == 0);
    printf("PASSED\n");
}

void test_logging() {
    printf("Testing logging system... ");
    net_log_set_level(NET_LOG_DEBUG);
    NET_LOG_I("This is an info message for testing.");
    NET_LOG_D("This is a debug message for testing.");
    NET_LOG_E("This is an error message for testing.");
    printf("PASSED\n");
}

void test_client_server_basic() {
    printf("Testing basic client-server communication... ");
    const char *port = "9101";
    const char *msg = "Hello, netlib!";
    
    pid_t pid = fork();
    if (pid == 0) {
        int listen_fd = net_start_server(NULL, port);
        assert(listen_fd != -1);
        int client_fd;
        assert(net_accept(listen_fd, &client_fd) == 1);
        char buf[64] = {0};
        assert(net_recv_all(client_fd, buf, strlen(msg)) != -1);
        assert(strcmp(buf, msg) == 0);
        close(client_fd);
        close(listen_fd);
        exit(0);
    } else {
        sleep(1);
        int sock_fd = net_start_client("127.0.0.1", port);
        assert(sock_fd != -1);
        assert(net_send_all(sock_fd, msg, strlen(msg)) == 1);
        close(sock_fd);
        int status;
        waitpid(pid, &status, 0);
        assert(WIFEXITED(status) && WEXITSTATUS(status) == 0);
    }
    printf("PASSED\n");
}

void test_messaging_protocol() {
    printf("Testing length-prefixed messaging... ");
    const char *port = "9107";
    const char *msg = "This is a length-prefixed message test.";
    
    pid_t pid = fork();
    if (pid == 0) {
        int listen_fd = net_start_server(NULL, port);
        assert(listen_fd != -1);
        int client_fd;
        assert(net_accept(listen_fd, &client_fd) == 1);
        
        char *recv_buf;
        size_t recv_len;
        assert(net_recv_msg(client_fd, &recv_buf, &recv_len) == 1);
        assert(recv_len == strlen(msg));
        assert(strcmp(recv_buf, msg) == 0);
        
        free(recv_buf);
        close(client_fd);
        close(listen_fd);
        exit(0);
    } else {
        sleep(1);
        int sock_fd = net_start_client("127.0.0.1", port);
        assert(sock_fd != -1);
        assert(net_send_msg(sock_fd, msg, strlen(msg)) == 1);
        close(sock_fd);
        int status;
        waitpid(pid, &status, 0);
        assert(WIFEXITED(status) && WEXITSTATUS(status) == 0);
    }
    printf("PASSED\n");
}

void test_udp_basic() {
    printf("Testing basic UDP transfer... ");
    const char *port = "9104";
    const char *msg = "UDP Message";
    
    pid_t pid = fork();
    if (pid == 0) {
        int sockfd = net_udp_start(NULL, port);
        assert(sockfd != -1);
        char buf[64] = {0};
        assert(net_udp_recvfrom(sockfd, buf, 64) > 0);
        assert(strcmp(buf, msg) == 0);
        close(sockfd);
        exit(0);
    } else {
        sleep(1);
        int sockfd = net_udp_start("127.0.0.1", NULL);
        assert(sockfd != -1);
        assert(net_udp_sendto(sockfd, "127.0.0.1", port, msg, strlen(msg)) == 1);
        close(sockfd);
        int status;
        waitpid(pid, &status, 0);
        assert(WIFEXITED(status) && WEXITSTATUS(status) == 0);
    }
    printf("PASSED\n");
}

void test_timeout() {
    printf("Testing socket timeout... ");
    const char *port = "9106";
    
    int listen_fd = net_start_server(NULL, port);
    assert(listen_fd != -1);
    
    pid_t pid = fork();
    if (pid == 0) {
        sleep(1);
        int sock_fd = net_start_client("127.0.0.1", port);
        assert(sock_fd != -1);
        sleep(3);
        close(sock_fd);
        exit(0);
    } else {
        int client_fd;
        assert(net_accept(listen_fd, &client_fd) == 1);
        assert(net_set_timeout(client_fd, 1) == 1);
        
        char buf[10];
        assert(net_recv_all(client_fd, buf, 10) == -1);
        
        close(client_fd);
        close(listen_fd);
        waitpid(pid, NULL, 0);
    }
    printf("PASSED\n");
}

int main() {
    net_log_set_level(NET_LOG_INFO);
    test_address_conversion();
    test_logging();
    test_client_server_basic();
    test_messaging_protocol();
    test_udp_basic();
    test_timeout();
    printf("All tests passed!\n");
    return 0;
}
