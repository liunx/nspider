/*
 * nspr_select.c
 */
#include <nspr_event.h>
#include <nspr_mem.h>

static fd_set master_read_fd_set;
static fd_set master_write_fd_set;
static fd_set work_read_fd_set;
static fd_set work_write_fd_set;
static int max_fd;
static unsigned int nevents;
static nspr_event_node_fd_t **event_node_fds;

static void nspr_select_repair_fd_sets(void);

static int nspr_select_init(int tmsec)
{
    nspr_event_node_fd_t **index;
    if ((index = nspr_malloc(NSPR_EVENT_FDS_SIZE)) == NULL) {
        return NSPR_ENOMEM;
    }
    event_node_fds = index;
    max_fd = -1;
    FD_ZERO(&master_read_fd_set);
    FD_ZERO(&master_write_fd_set);
    nevents = 0;
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
            FD_SET(node_fd->fd, &master_read_fd_set);
            break;
        case NSPR_EVENT_TYPE_WRITE:
            FD_SET(node_fd->fd, &master_write_fd_set);
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
    event_node_fds[nevents] = node_fd;
    node_fd->index = nevents;
    nevents++;

    return NSPR_OK;
}

static int nspr_select_del(nspr_event_node_fd_t *node_fd)
{
    nspr_event_node_fd_t *n;

    if (node_fd->index == NSPR_INVALID_EVENT_FD_INDEX)
        return NSPR_OK;

    switch (node_fd->event_type) {
        case NSPR_EVENT_TYPE_READ:
            FD_CLR(node_fd->fd, &master_read_fd_set);
            break;
        case NSPR_EVENT_TYPE_WRITE:
            FD_CLR(node_fd->fd, &master_write_fd_set);
            break;
        case NSPR_EVENT_TYPE_ERROR:
            break;
        default:
            nspr_log_error("Unkown event type %d\n", node_fd->event_type);
            break;
    }

    if (max_fd == node_fd->fd) {
        max_fd = -1;
    }

    if (node_fd->index < --nevents) {
        n = event_node_fds[nevents];
        event_node_fds[node_fd->index] = n;
        n->index = node_fd->index;
    }
    node_fd->index = NSPR_INVALID_EVENT_FD_INDEX;

    return NSPR_OK;
}

static void nspr_select_exit(void)
{
    nspr_free(event_node_fds);
    FD_ZERO(&master_read_fd_set);
    FD_ZERO(&master_write_fd_set);
}

static int nspr_select_process_events(unsigned long timer)
{
    int ready, nready;
    nspr_event_node_fd_t *node_fd;
    unsigned int i, found;
    struct timeval tv, *tp;

    if (max_fd == -1) {
        for (i = 0; i < nevents; i++) {
            node_fd = event_node_fds[i];
            if (max_fd < node_fd->fd) {
                max_fd = node_fd->fd;
            }
        }
    }

    if (timer == NSPR_TIMER_INFINITE) {
        tp = NULL;

    } else {
        tv.tv_sec = (long) (timer / 1000);
        tv.tv_usec = (long) ((timer % 1000) * 1000);
        tp = &tv;
    }

    nready = 0;
    work_read_fd_set = master_read_fd_set;
    work_write_fd_set = master_write_fd_set;

    ready = select(max_fd + 1, &work_read_fd_set, &work_write_fd_set, NULL, tp);

    nspr_time_update();

    if (ready == -1) {
        if (errno == EBADF) {
            nspr_select_repair_fd_sets();
        }
        else if (errno == EINTR) {
            // TODO add signal process
        }
        else {
            return NSPR_ERROR;
        }
    }

    for (i = 0; i < nevents; i++) {
        node_fd = event_node_fds[i];
        found = 0;

        if (node_fd->event_type == NSPR_EVENT_TYPE_READ) {
            if (FD_ISSET(node_fd->fd, &work_read_fd_set)) {
                if (node_fd->read) {
                    node_fd->read(node_fd);
                }
                found = 1;
            }
        }
        else if (node_fd->event_type == NSPR_EVENT_TYPE_WRITE) {
            if (FD_ISSET(node_fd->fd, &work_write_fd_set)) {
                if (node_fd->write) {
                    node_fd->write(node_fd);
                }
                found = 1;
            }
        }

        if (found) {
            nready++;
        }
    }

    if (ready != nready) {
        // FIXME fix it!
    }

    return NSPR_OK;
}

/*
 * come from nginx ngx_select_module.c:ngx_select_repair_fd_sets
 */
static void nspr_select_repair_fd_sets(void)
{
    unsigned int i;
    int n;
    socklen_t len;
    nspr_event_node_fd_t *node_fd;

    for (i = 0; i < nevents; i++) {
        node_fd = event_node_fds[i];
        if (FD_ISSET(node_fd->fd, &master_read_fd_set)) {
            len = sizeof(int);
            if (getsockopt(node_fd->fd, SOL_SOCKET, SO_TYPE, &n, &len) == -1) {
                FD_CLR(node_fd->fd, &master_read_fd_set);
                node_fd->error(node_fd);
            }
        }

        if (FD_ISSET(node_fd->fd, &master_write_fd_set)) {
            len = sizeof(int);
            if (getsockopt(node_fd->fd, SOL_SOCKET, SO_TYPE, &n, &len) == -1) {
                FD_CLR(node_fd->fd, &master_write_fd_set);
                node_fd->error(node_fd);
            }
        }
    }

    max_fd = -1;
}

nspr_event_handler_t nspr_event_handler = {
    .init = nspr_select_init,
    .add = nspr_select_add,
    .process_events = nspr_select_process_events,
    .del = nspr_select_del,
    .exit = nspr_select_exit,
    .data = NULL,
};
