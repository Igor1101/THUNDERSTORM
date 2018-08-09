/*
 * Copyright (C) 2018  Igor Muravyov <igor.muravyov.2015@gmail.com>
 */
#include <stdint.h>
#include <kstdio.h>
#include <TH/lld.h>
#include <TH/sysvars.h>
#include <TH/die.h>
#include <asm/memory_mapping.h>

/* legacy, removing this */
/*uintptr_t *map_video(volatile void *addr)
{                               
                                   in arch/x86_64/boot/boot.asm: 8 * 2MB
                                   used for mapping videoaddr/framebuffer
                                   to kernel memory
                                   virtual addr = 2MB * (PG_SIZE - 8)/4 = 0x3fe00000;
                                

        uint64_t volatile videoaddr = (uint64_t) addr | 0b10000011;
        volatile uint64_t *volatile map_addr =
            (void *)&p2_table + PG_SIZE - 8 * sizeof(uint64_t);
        (*map_addr) = videoaddr;        // 1st 2 MB`s : 1
        videoaddr += 2 << 20;
        (*(map_addr + 1)) = videoaddr;  // second 2 MB`s : 2
        videoaddr += 2 << 20;
        (*(map_addr + 2)) = videoaddr;  // : 3
        videoaddr += 2 << 20;
        (*(map_addr + 3)) = videoaddr;  // : 4
        videoaddr += 2 << 20;
        (*(map_addr + 4)) = videoaddr;  // : 5
        videoaddr += 2 << 20;
        (*(map_addr + 5)) = videoaddr;  // : 6
        videoaddr += 2 << 20;
        (*(map_addr + 6)) = videoaddr;  // : 7
        videoaddr += 2 << 20;
        (*(map_addr + 7)) = videoaddr;  // : 8

        return (uintptr_t *)
            ((uintptr_t) (2 << 20) * (PG_SIZE / sizeof(uintptr_t) - 8));
}*/

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
        if(mmap_current >= MMAP_END) {
                return NULL;
        }
        void* virt_addr = (void*)(uintptr_t)(_2MBYTE * mmap_current);
        uint32_t mmap_old = mmap_current;
        _2MBYTE_ALIGN(length);
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
