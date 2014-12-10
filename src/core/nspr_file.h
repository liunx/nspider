#ifndef _NSPR_FILE_H_INCLUDED_
#define _NSPR_FILE_H_INCLUDED_
#include <nspr_common.h>
ssize_t nspr_file_read(int fd, void *buf, size_t count);
ssize_t nspr_file_write(int fd, const void *buf, size_t count);
int nspr_file_close(int fd);
#endif /* _NSPR_FILE_H_INCLUDED_ */
