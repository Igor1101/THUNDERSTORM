/*
 * Copyright (C) 2018-2019  Igor Muravyov <igor.muravyov.2015@gmail.com>
 */
#ifndef KSTDIOB
#define KSTDIOB

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdarg.h>
#include <stddef.h>
#include <compiler_opt.h>

        void kprintf(const char *fmt, ...);
        void kvprintf(const char *fmt, va_list args);
        void kputchar(int8_t chr);
        LIKELY int vsnprintf (char * s, size_t n, 
                        const char * fmt, va_list args );
        /* Legacy */
        LIKELY WEAK int vsnprintf (char * s, size_t n, 
                        const char * fmt, va_list args );
        LIKELY void kputs(const char *s);

#ifdef __cplusplus
}
#endif
#endif                          /* KSTDIOB */
