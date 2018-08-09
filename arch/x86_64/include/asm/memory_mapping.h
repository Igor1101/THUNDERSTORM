/*
 * Copyright (C) 2018  Igor Muravyov <igor.muravyov.2015@gmail.com>
 */
#ifndef MEMMAP
#define MEMMAP

#include <stdint.h>
#define PG_SIZE 4096
#define PG_SIZE_QW (PG_SIZE / sizeof(uintptr_t) )
#define _2MBYTE (2<<20)
#define _MEM_AVAILABLE (0b10000011)
#define _2MBYTE_ALIGN(X) while( ( (X) % _2MBYTE ) != 0) X++

/* kernel static memory end */
#define KERNEL_MEM_END (PG_SIZE_QW / 2 - 1)
#define MMAP_END (PG_SIZE_QW - 1)
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

/**
 * this is far more different from POSIX mmap
 * void *kmmap(void *addr, size_t length)
 * @addr: address, which should be mapped (must be pg aligned)
 * @length: length of mmapped area
 */
LIKELY void *kmmap(void *addr, size_t length);

#endif                          /*MEMMAP */
