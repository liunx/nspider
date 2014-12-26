#ifndef _NSPR_INET_H_INCLUDED_
#define _NSPR_INET_H_INCLUDED_
#include <nspr_common.h>
#define BACKLOG 128

int nspr_create_ipv4_tcp_listen(const char *addr, const int port);
int nspr_create_ipv4_tcp_accept(int fd);
int nspr_create_ipv4_tcp_connect(const char *addr, const int port);
int nspr_get_connect_status(const int fd);
int nspr_get_peer_name(const int fd, char *addr, int *port);
int nspr_get_socket_name(const int fd, char *addr, int *port);
int nspr_get_iface_info(const char *iface, char *info);
#endif /* _NSPR_INET_H_INCLUDED_ */
