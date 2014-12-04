/*
 * nspr_event.c
 */
#include <nspr_event.h>

void nspr_event_process(void)
{
    for(;;) {
        // TODO add event
	// TODO handle event
	// TODO clear event
    }
}

int nspr_event_init(void)
{
    nspr_nspider_process = nspr_event_process;
    nspr_event_handler.init(1000);
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
