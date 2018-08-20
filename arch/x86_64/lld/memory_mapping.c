/*
 * Copyright (C) 2018  Igor Muravyov <igor.muravyov.2015@gmail.com>
 */
#include <stdint.h>
#include <kstdio.h>
#include <TH/lld.h>
#include <TH/sysvars.h>
#include <TH/die.h>
#include <asm/memory_mapping.h>

/**
 * void* last_addr(void)
 * return: last mapped kernel address
 */
void* last_addr(void)
{
        return (void*)p2_table[KERNEL_MEM_END];
}

static uint32_t mmap_current = KERNEL_MEM_END + 1;

/**
 * this is far more different from POSIX mmap
 * void *kmmap(void *addr, size_t length)
 * @addr: address, which should be mapped
 * @length: length of mmapped area
 */
LIKELY void *kmmap(void *addr, size_t length)
{
        if(addr < last_addr()) {
                /* we are mapping kernel memory to kernel memory, 
                 * aren`t we?
                 */
                return addr;
        }
        if(mmap_current >= MMAP_END) {
                return NULL;
        }
        void* virt_addr = (void*)(uintptr_t)(_2MBYTE * mmap_current);
        uint32_t mmap_old = mmap_current;
        length = _2MBYTE_ALIGN(length);
        size_t _2mbyte_length = length / _2MBYTE;
        do {
                p2_table[mmap_current++] = (uintptr_t)addr|_MEM_AVAILABLE;
                addr += _2MBYTE;
                if(mmap_current >= MMAP_END) {
                        mmap_current = mmap_old;
                        return NULL;
                }
        }
        while(_2mbyte_length-- > 0);
        return virt_addr;
}


