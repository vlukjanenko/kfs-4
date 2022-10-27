#ifndef BASELIB_H
# define BASELIB_H

#include "stddef.h"

size_t  strlen(const char* str);
void    *memcpy(void *dst, const void *src, size_t n);
void    *memmove(void *dst, const void *src, size_t len);

#endif