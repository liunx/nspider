/*
 * nspr_mem.c
 */
#include <nspr_mem.h>

void *nspr_malloc(size_t size)
{
    return malloc(size);
}

void nspr_free(void *ptr)
{
    return free(ptr);
}

void *nspr_calloc(size_t nmemb, size_t size)
{
    return calloc(nmemb, size);
}

void *realloc(void *ptr, size_t size)
{
    return realloc(ptr, size);
}
