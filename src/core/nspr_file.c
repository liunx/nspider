/*
 * nspr_file.c
 */
#include <nspr_file.h>

ssize_t nspr_file_read(int fd, void *buf, size_t count)
{
    return read(fd, buf, count);
}

ssize_t nspr_file_write(int fd, const void *buf, size_t count)
{
    return write(fd, buf, count);
}

int nspr_file_close(int fd)
{
    return close(fd);
}
