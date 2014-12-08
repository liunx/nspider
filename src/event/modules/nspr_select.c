/*
 * nspr_select.c
 */
#include <nspr_event.h>

static fd_set read_fd_set;
static fd_set write_fd_set;
static int max_fd;

static int nspr_select_init(int tmsec)
{
    max_fd = -1;
    FD_ZERO(&read_fd_set);
    FD_ZERO(&write_fd_set);
    return NSPR_OK;
}

static int nspr_select_add(nspr_event_node_fd_t *node_fd)
{
    // check fd > 0 or we'll FD_SET failed
    if (node_fd->fd < 0) {
        nspr_log_error("Invalid fd -- %d\n", node_fd->fd);
        return NSPR_EBADF;
    }

    switch (node_fd->event_type) {
        case NSPR_EVENT_TYPE_READ:
            FD_SET(node_fd->fd, &read_fd_set);
            break;
        case NSPR_EVENT_TYPE_WRITE:
            FD_SET(node_fd->fd, &write_fd_set);
            break;
        case NSPR_EVENT_TYPE_ERROR:
            nspr_log_error("We do not need execption error set!\n");
            return NSPR_EUNDEF;
        default:
            nspr_log_error("Unkown event type %d\n", node_fd->event_type);
            return NSPR_EUNDEF;
    }
    if (max_fd != -1 && max_fd < node_fd->fd) {
        max_fd = node_fd->fd;
    }
    return NSPR_OK;
}

static int nspr_select_del(nspr_event_node_fd_t *node_fd)
{
    // check fd > 0 or we'll FD_SET failed
    if (node_fd->fd < 0) {
        nspr_log_error("Invalid fd -- %d\n", node_fd->fd);
        return NSPR_EBADF;
    }

    switch (node_fd->event_type) {
        case NSPR_EVENT_TYPE_READ:
            FD_CLR(node_fd->fd, &read_fd_set);
            break;
        case NSPR_EVENT_TYPE_WRITE:
            FD_CLR(node_fd->fd, &write_fd_set);
            break;
        case NSPR_EVENT_TYPE_ERROR:
            break;
        default:
            nspr_log_error("Unkown event type %d\n", node_fd->event_type);
            break;
    }
    return NSPR_OK;
}

static void nspr_select_exit(void)
{
    FD_ZERO(&read_fd_set);
    FD_ZERO(&write_fd_set);
}

static void nspr_select_process_events(int tmsec)
{
    // FIXME add timer
}

nspr_event_handler_t nspr_event_handler = {
    .init = nspr_select_init,
    .add = nspr_select_add,
    .process_events = nspr_select_process_events,
    .del = nspr_select_del,
    .exit = nspr_select_exit,
    .data = NULL,
};
