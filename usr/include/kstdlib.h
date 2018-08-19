/*
 * Copyright (C) 2018  Igor Muravyov <igor.muravyov.2015@gmail.com>
 */
#ifndef KSTDLIB_H
#define KSTDLIB_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>

#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0

#define ITOA_BUF_SIZE 33

        void itoa(char *buf, intptr_t base, intptr_t value);
        void itoap(char *buf, size_t width, intptr_t base, uintptr_t value);
        void utoa(char *buf, uintptr_t value);
        /* kernel memory allocation */
#define MAX_RAM_ALLOCS 10
/**
 * int kalloc_init(void)
 * @return: SUCCESS if OK
 * FAILURE if memory cannot be allocated
 */
        int kalloc_init(void);
        void *kcalloc(size_t size);
        void kfree(void *addr);
        /*acpi function */
        uint32_t strtoul(const char *String, char **Terminator, uint32_t Base);

#ifdef __cplusplus
}
#endif

#endif                          /* KSTDLIB_H */
