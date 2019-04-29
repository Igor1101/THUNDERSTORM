/*
 * Copyright (C) 2018-2019  Igor Muravyov <igor.muravyov.2015@gmail.com>
 */
#ifndef KSTRING_H
#define KSTRING_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <compiler_opt.h>

char *strncpy(char *dest, const char *src, size_t n);
/* copy (ptr * 16) -aligned data (faster than memcpy ) */
void *kmemcpy_ptr(void *dest, const void *src, size_t len);
/* memset with progress */
void *kmemset_show(void *dest, register int val, register size_t len);
void *memset(void *dest, register int val, register size_t len);

void *memcpy(void *dest, const void *src, size_t len);
void *memmove(void *dest, const void *src, size_t len);
size_t strlen(const char *s);
int strcmp(const char * s1, const char * s2);
int strncmp(const char * s1, const char * s2, size_t n);
char *strcat(char * restrict dest, const char * restrict src);
char *strncat(char * restrict dest, const char * restrict src, size_t n);
char *strcpy(char * restrict dest, const char * restrict src);
int memcmp(const void * s1, const void * s2,size_t n);
char *strchr(const char *s, int c);
char *strstr(char *s1, char *s2);

#ifdef __cplusplus
}
#endif

#endif                          /* KSTRING_H */
