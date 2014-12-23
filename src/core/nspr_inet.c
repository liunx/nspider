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

int nspr_create_ipv4_tcp_accept(int fd)
{
    //int  nb = 1;
    struct sockaddr_in client_addr;
    socklen_t sin_size = sizeof(client_addr);
    int new_fd = accept(fd, (struct sockaddr *)&client_addr, &sin_size);
    if (new_fd == -1) {
        return -1;
    }
    // set sock_fd nonblocking
    //ioctl(new_fd, FIONBIO, &nb);

    return new_fd;
}

int nspr_create_ipv4_tcp_connect(const char *addr, const int port)
{
    int sock_fd = -1;
    int  nb = 1;
    int ret;
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

    if ((ret = connect(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr))) == -1) {
        if (errno == EINPROGRESS) {
            return sock_fd;
        }
        else {
            return NSPR_ERROR;
        }
    }

    return sock_fd;
}

int nspr_get_connect_status(const int fd)
{
    int err = 0;
    socklen_t len;
    getsockopt(fd, SOL_SOCKET, SO_ERROR, (void *) &err, &len);
    if (err == 0) {
        return NSPR_OK;
    }
    else {
        return NSPR_ERROR;
    }
}

int nspr_get_peer_name(const int fd, char *addr, int *port)
{
    struct sockaddr_in peer;
    socklen_t peer_len = sizeof(peer);
    if (getpeername(fd, (struct sockaddr *) &peer, &peer_len) < 0) {
        return NSPR_ERROR;
    } else {
        strcpy(addr, inet_ntoa(peer.sin_addr));
        *port = ntohs(peer.sin_port);
    }
    return NSPR_OK;
}

int nspr_get_socket_name(const int fd, char *addr, int *port)
{
    struct sockaddr_in local;
    socklen_t local_len = sizeof(local);
    if (getsockname(fd, (struct sockaddr*) &local, &local_len) < 0) {
        return NSPR_ERROR;
    } else {
        strcpy(addr, inet_ntoa(local.sin_addr));
        *port = ntohs(local.sin_port);
    }
    return NSPR_OK;
}
