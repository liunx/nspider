/*
 * nspr_log.c
 */
#include <nspr_log.h>

int nspr_log_init(void)
{
    nspr_log_error = printf;
    return NSPR_OK;
}

void nspr_log_exit(void)
{
    // do nothing
}

nspr_nspider_t nspr_log = {
    .id =  NSPR_ID_LOG,
    .init = nspr_log_init,
    .exit = nspr_log_exit,
};
