#ifndef _NSPR_SIGNAL_H_INCLUDED_
#define _NSPR_SIGNAL_H_INCLUDED_
#include <nspr_common.h>

#define NSPR_SIGNAL_MAX 64

struct nspr_event_signal_s {
    unsigned long id;
    union {
        int signum;
        int index;
    };
    void (*handler)(struct nspr_event_signal_s *);
};

typedef struct nspr_event_signal_s nspr_event_signal_t;

extern sig_atomic_t nspr_event_signal;
extern sig_atomic_t nspr_event_signal_signum;

void nspr_event_add_signal(nspr_event_signal_t *sig);
void nspr_event_del_signal(nspr_event_signal_t *sig);
void nspr_event_signal_process(void);
#endif /* _NSPR_SIGNAL_H_INCLUDED_ */
