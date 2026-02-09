#include "addr.h"
#include "client.h"
#include "server.h"
#include "udp.h"
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

void test_large_data_transfer() {
    printf("Testing large data transfer (1MB)... ");
    const char *port = "9102";
    size_t size = 1024 * 1024; // 1MB
    char *data = malloc(size);
    assert(data != NULL);
    for (size_t i = 0; i < size; i++) data[i] = (char)(i % 256);

    pid_t pid = fork();
    if (pid == 0) {
        int listen_fd = net_start_server(NULL, port);
        assert(listen_fd != -1);
        int client_fd;
        assert(net_accept(listen_fd, &client_fd) == 1);
        char *recv_buf = malloc(size);
        assert(net_recv_all(client_fd, recv_buf, size) != -1);
        assert(memcmp(data, recv_buf, size) == 0);
        free(recv_buf);
        close(client_fd);
        close(listen_fd);
        exit(0);
    } else {
        sleep(1);
        int sock_fd = net_start_client("127.0.0.1", port);
        assert(sock_fd != -1);
        assert(net_send_all(sock_fd, data, size) == 1);
        close(sock_fd);
        int status;
        waitpid(pid, &status, 0);
        assert(WIFEXITED(status) && WEXITSTATUS(status) == 0);
    }
    free(data);
    printf("PASSED\n");
}

void test_file_transfer() {
    printf("Testing simulated file transfer... ");
    const char *port = "9103";
    const char *test_data = "Simulated file content with various characters: !@#$%^&*()_+";
    size_t size = strlen(test_data);

    pid_t pid = fork();
    if (pid == 0) {
        int listen_fd = net_start_server(NULL, port);
        assert(listen_fd != -1);
        int client_fd;
        assert(net_accept(listen_fd, &client_fd) == 1);
        char *recv_buf = malloc(size + 1);
        assert(net_recv_all(client_fd, recv_buf, size) != -1);
        recv_buf[size] = '\0';
        assert(strcmp(test_data, recv_buf) == 0);
        free(recv_buf);
        close(client_fd);
        close(listen_fd);
        exit(0);
    } else {
        sleep(1);
        int sock_fd = net_start_client("127.0.0.1", port);
        assert(sock_fd != -1);
        assert(net_send_all(sock_fd, test_data, size) == 1);
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
    if (listen_fd == -1) {
        perror("net_start_server");
        assert(listen_fd != -1);
    }
    
    pid_t pid = fork();
    if (pid == 0) {
        // Parent process (Client)
        sleep(1);
        int sock_fd = net_start_client("127.0.0.1", port);
        assert(sock_fd != -1);
        sleep(3); // Stay connected but don't send data
        close(sock_fd);
        exit(0);
    } else {
        // Server process
        int client_fd;
        assert(net_accept(listen_fd, &client_fd) == 1);
        assert(net_set_timeout(client_fd, 1) == 1);
        
        char buf[10];
        int result = net_recv_all(client_fd, buf, 10);
        
        // Should return -1 due to timeout
        assert(result == -1);
        
        close(client_fd);
        close(listen_fd);
        waitpid(pid, NULL, 0);
    }
    printf("PASSED\n");
}

int main() {
    test_address_conversion();
    test_client_server_basic();
    test_large_data_transfer();
    test_file_transfer();
    test_udp_basic();
    sleep(1);
    test_timeout();
    printf("All tests passed!\n");
    return 0;
}
