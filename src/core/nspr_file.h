#ifndef _NSPR_FILE_H_INCLUDED_
#define _NSPR_FILE_H_INCLUDED_
#include <nspr_common.h>
ssize_t nspr_file_read(int fd, void *buf, size_t count);
ssize_t nspr_file_write(int fd, const void *buf, size_t count);
int nspr_file_close(int fd);
FILE *nspr_file_popen(const char *cmd, const char *mode);
int nspr_file_pclose(FILE *fp);
#endif /* _NSPR_FILE_H_INCLUDED_ */
