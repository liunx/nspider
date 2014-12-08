#ifndef _NSPR_COMMON_H_INCLUDED_
#define _NSPR_COMMON_H_INCLUDED_
#include <nspider_config.h>
#ifdef NSPR_HAVE_OS_LINUX
#include <stdio.h>
#include <stdlib.h>
#include <nspr_linux.h>
#endif

#define  NSPR_OK            0
#define  NSPR_ERROR        -1
#define  NSPR_EBADF        -2
#define  NSPR_EUNDEF       -3
#define  NSPR_ENOMEM       -4

#define NSPR_ID_LOG         0x1000
#define NSPR_ID_EVENT       0x1001
#define NSPR_ID_LUAPI       0x1002

#endif /* _NSPR_COMMON_H_INCLUDED_ */
