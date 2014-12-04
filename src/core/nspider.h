#ifndef _NSPIDER_H_INCLUDED_
#define _NSPIDER_H_INCLUDED_
#include <nspider_config.h>
#include <nspr_common.h>

struct nspr_nspider_s {
    int id;
    int (*init)(void);
    void (*exit)(void);
};

typedef struct nspr_nspider_s nspr_nspider_t;

// we'll provide work to others
extern void (*nspr_nspider_process)(void);
extern int (*nspr_log_error)(const char *, ...);
extern nspr_nspider_t  *nspr_nspider_modules[];

#endif /* _NSPIDER_H_INCLUDED_ */
