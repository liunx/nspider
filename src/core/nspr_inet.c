/*
 * nspr_inet.c
 */
#include <nspr_inet.h>

int nspr_create_ipv4_tcp_listen(const char *addr, const int port)
{
    int sock_fd = -1;
    int yes = 1;
    struct sockaddr_in server_addr;

    if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        return NSPR_ERROR;
    }

    if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
        goto err;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(addr);
    memset(server_addr.sin_zero, '\0', sizeof(server_addr.sin_zero));

    if (bind(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        goto err;
    }

    if (listen(sock_fd, BACKLOG) == -1) {
        goto err;
    }

    return sock_fd;
err:
    close(sock_fd);
    return NSPR_ERROR;
}

int nspr_create_ipv4_tcp_connection(const char *addr, const int port)
{
    int sock_fd = -1;
    int  nb = 1;
    struct sockaddr_in server_addr;

    if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        return NSPR_ERROR;
    }

    memset(&server_addr, 0, sizeof(struct sockaddr_in));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(addr);
    memset(server_addr.sin_zero, '\0', sizeof(server_addr.sin_zero));

    // set sock_fd nonblocking
    ioctl(sock_fd, FIONBIO, &nb);

    connect(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));

    return sock_fd;
}
