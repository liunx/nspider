/*
 * nspr_signal.c
 */
#include <nspr_signal.h>

sig_atomic_t nspr_event_signal = 0;
sig_atomic_t nspr_event_signal_signum = 0;

static nspr_event_signal_t *signals[NSPR_SIGNAL_MAX];

static void sighandler(int signum)
{
    nspr_event_signal_signum = signum;
}

void nspr_event_add_signal(nspr_event_signal_t *sig)
{
    signals[sig->index] = sig;
    signal(sig->signum, sighandler);
}

void nspr_event_del_signal(nspr_event_signal_t *sig)
{
    signal(sig->signum, SIG_IGN);
    signals[sig->index] = NULL;
}

void nspr_event_signal_process(void)
{
    nspr_event_signal_t *sig;
    sig = signals[nspr_event_signal_signum];
    if (sig) {
        sig->handler(sig);
    }
}
