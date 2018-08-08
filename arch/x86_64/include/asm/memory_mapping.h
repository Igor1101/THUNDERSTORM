/*
 * Copyright (C) 2018  Igor Muravyov <igor.muravyov.2015@gmail.com>
 */
#ifndef MEMMAP
#define MEMMAP

#include <stdint.h>
#define PG_SIZE 4096
#define PG_SIZE_QW (PG_SIZE / sizeof(uintptr_t))
/* kernel static memory end */
#define KERNEL_MEM_END (PG_SIZE_QW / 2 - 1)
extern uintptr_t p4_table[PG_SIZE_QW];
extern uintptr_t p3_table[PG_SIZE_QW];
extern uintptr_t p2_table[PG_SIZE_QW];
extern uintptr_t p1_table[PG_SIZE_QW];

/**
 * void* last_addr(void)
 * return: last mapped kernel address
 */
void* last_addr(void);

uintptr_t *map_video(volatile void *addr);

#endif                          /*MEMMAP */
