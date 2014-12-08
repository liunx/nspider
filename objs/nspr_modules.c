#include <nspider.h>
extern nspr_nspider_t nspr_log;
extern nspr_nspider_t nspr_event;
extern nspr_nspider_t nspr_luapi;

nspr_nspider_t *nspr_nspider_modules[] = {
    &nspr_log,
    &nspr_event,
    &nspr_luapi,
    NULL,
};
