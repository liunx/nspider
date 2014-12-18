#ifndef _NSPR_TIMER_H_INCLUDED_
#define _NSPR_TIMER_H_INCLUDED_
#include <nspr_common.h>

#define NSPR_TIMER_INVALID_INDEX -1
#define TIMERS_SIZE 128
#define NSPR_TIMER_INFINITE (unsigned long) -1
#define nspr_gettimeofday(tp)	gettimeofday(tp, NULL)

struct nspr_event_timer_s {
    int index;
    unsigned long timer;
    unsigned long delta;
    unsigned int id;
    void (*handler)(struct nspr_event_timer_s *);
};

typedef struct nspr_event_timer_s nspr_event_timer_t;

extern volatile unsigned long nspr_current_msec;

void nspr_time_update(void);
unsigned long nspr_event_find_timer(void);
void nspr_event_expire_timers(void);
void nspr_event_add_timer(nspr_event_timer_t *timer);
void nspr_event_del_timer(nspr_event_timer_t *timer);
#endif /* _NSPR_TIMER_H_INCLUDED_ */
