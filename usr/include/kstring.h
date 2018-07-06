/*
 * Copyright (C) 2018  Igor Muravyov <igor.muravyov.2015@gmail.com>
 */
#ifndef KSTRING_H
#define KSTRING_H

#include <stddef.h>
#include <gcc_opt.h>

char *strncpy(char *dest, const char *src, size_t n);
/* copy (ptr * 16) -aligned data (faster than memcpy ) */
void *kmemcpy_ptr(void *dest, const void *src, size_t len);
/* memset with progress */
void *kmemset_show(void *dest, register int val, register size_t len);
void *memset(void *dest, register int val, register size_t len);

void *memcpy(void *dest, const void *src, size_t len);
size_t strlen(const char *s);

#endif                          /* KSTRING_H */
