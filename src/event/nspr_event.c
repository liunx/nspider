/*
 * nspr_event.c
 */
#include <nspr_event.h>

void nspr_event_handler(void)
{
    for(;;) {
        // do works
    }
}

int nspr_event_init(void)
{
    nspr_handler = nspr_event_handler;
    return NSPR_OK;
}

void nspr_event_exit(void)
{
}

nspr_nspider_t nspr_event = {
    .id =  NSPR_ID_EVENT,
    .init = nspr_event_init,
    .exit = nspr_event_exit,
};
