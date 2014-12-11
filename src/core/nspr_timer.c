/*
 * nspr_timer.c
 */
#include <nspr_timer.h>

volatile unsigned long nspr_current_msec;
// we just support little size timers
static nspr_event_timer_t *timer_list[TIMERS_SIZE];
static int ntimers = 0;

void nspr_time_update(void)
{
    unsigned long sec, msec;
    struct timeval   tv;

    nspr_gettimeofday(&tv);
    sec = tv.tv_sec;
    msec = tv.tv_usec / 1000;

    nspr_current_msec = (unsigned long) sec * 1000 + msec;
}

void nspr_event_expire_timers(void)
{
    int i;
    nspr_event_timer_t *timer;
    unsigned long delta;

    for (i = 0; i < ntimers; i++) {
        timer = timer_list[i];
        delta = nspr_current_msec - timer->delta;
        if (delta > timer->timer) {
            timer->handler(timer);
            timer->delta = nspr_current_msec;
        }
    }
}

unsigned long nspr_event_find_timer(void)
{
    int i, j;
    unsigned long deltas[TIMERS_SIZE];
    unsigned long delta;
    nspr_event_timer_t *timer;

    for (i = 0, j = 0; i < ntimers; i++) {
        timer = timer_list[i];
        delta = nspr_current_msec - timer->delta;
        if (delta < timer->timer) {
            deltas[j++] = delta;
        }
    }

    // get the latest delta time
    if (j > 0) {
        delta = 0;
        for (i = 0; i < j; i++) {
            if (delta < deltas[i]) {
                delta = deltas[i];
            }
        }
    }
    else {
        delta = NSPR_TIMER_INFINITE;
    }

    return delta;
}

void nspr_event_add_timer(nspr_event_timer_t *timer)
{
    nspr_time_update();
    timer->delta = nspr_current_msec;
    timer_list[ntimers] = timer;
    timer->index = ntimers;
    ntimers++;
}

void nspr_event_del_timer(nspr_event_timer_t *timer)
{
    nspr_event_timer_t *t;
    if (timer->index == NSPR_TIMER_INVALID_INDEX) 
        return;

    if (timer->index < --ntimers) {
        t = timer_list[ntimers];
        timer_list[timer->index] = t;
        t->index = timer->index;
    }
    timer->index = NSPR_TIMER_INVALID_INDEX;
}
