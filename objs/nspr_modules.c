#include <nspider.h>
extern nspr_nspider_t nspr_log;
extern nspr_nspider_t nspr_event;

nspr_nspider_t *nspr_nspider_modules[] = {
    &nspr_log,
    &nspr_event,
    NULL,
};
