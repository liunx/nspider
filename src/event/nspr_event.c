/*
 * nspr_event.c
 */
#include <nspr_event.h>

static int event_loop = 1;

void nspr_event_break(void)
{
    event_loop = 0;
}

void nspr_event_process(void)
{
    int ret;
    unsigned long delta, timer;

    while (event_loop) {
        timer = nspr_event_find_timer();
        delta = nspr_current_msec;
        ret = (int) nspr_event_handler.process_events(timer);
        delta = nspr_current_msec - delta;

        if (delta) {
            nspr_event_expire_timers();
        }
        // check signal
        if (nspr_event_signal) {
            nspr_event_signal_process();
            nspr_event_signal = 0;
        }
        if (ret != NSPR_OK) {
            break;
        }
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
    nspr_event_handler.exit();
}

nspr_nspider_t nspr_event = {
    .id =  NSPR_ID_EVENT,
    .init = nspr_event_init,
    .exit = nspr_event_exit,
};
