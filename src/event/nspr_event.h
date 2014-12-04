#ifndef _NSPR_EVENT_H_INCLUDED_
#define _NSPR_EVENT_H_INCLUDED_
#include <nspider_config.h>
#include <nspr_common.h>
#include <nspider.h>

// event type
#define NSPR_EVENT_TYPE_READ    0
#define NSPR_EVENT_TYPE_WRITE   1
#define NSPR_EVENT_TYPE_ERROR   2
#define NSPR_EVENT_TYPE_TIMER   3
#define NSPR_EVENT_TYPE_SIGNAL  4

// for both read/write
#define NSPR_EVENT_FD_STATUS_OK             0
#define NSPR_EVENT_FD_STATUS_EAGAIN         1
#define NSPR_EVENT_FD_STATUS_EBADF          2
#define NSPR_EVENT_FD_STATUS_EFAULT         3
#define NSPR_EVENT_FD_STATUS_EINTR          4
#define NSPR_EVENT_FD_STATUS_EINVAL         5
#define NSPR_EVENT_FD_STATUS_EIO            6
#define NSPR_EVENT_FD_STATUS_EISDIR         7
// specific for write
#define NSPR_EVENT_FD_STATUS_EDESTADDRREQ   8
#define NSPR_EVENT_FD_STATUS_EDQUOT         9
#define NSPR_EVENT_FD_STATUS_EBIG           10
#define NSPR_EVENT_FD_STATUS_ENOSPC         11
#define NSPR_EVENT_FD_STATUS_EPIPE          12
// specific for socket
// TODO add error status for send/sendto recv/recvfrom

// fd node
struct nspr_event_node_fd_s {
    int fd;
    int event_type;
    // handlers
    void (*read)(struct nspr_event_node_fd_s *);
    void (*write)(struct nspr_event_node_fd_s *);
    void (*error)(struct nspr_event_node_fd_s *);
    void *data;
};
typedef struct nspr_event_node_fd_s nspr_event_node_fd_t;

struct nspr_event_handler_s {
    int (*init)(int );
    int (*add)(nspr_event_node_fd_t *);
    void (*process_events)(int );
    int (*del)(nspr_event_node_fd_t *);
    void (*exit)(void);
    void *data;
};
typedef struct nspr_event_handler_s nspr_event_handler_t;

extern nspr_event_handler_t nspr_event_handler;
#endif /* _NSPR_EVENT_H_INCLUDED_ */
