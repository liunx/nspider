#ifndef _NSPR_MEM_H_INCLUDED_
#define _NSPR_MEM_H_INCLUDED_
#include <nspr_common.h>

void *nspr_malloc(size_t size);
void nspr_free(void *ptr);
void *nspr_calloc(size_t nmemb, size_t size);
void *realloc(void *ptr, size_t size);

#endif /* _NSPR_MEM_H_INCLUDED_ */
