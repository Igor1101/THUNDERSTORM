/*
 * Copyright (C) 2018  Igor Muravyov <igor.muravyov.2015@gmail.com>
 */
#include <stdint.h>
#include <kstdio.h>
#include <TH/lld.h>
#include <TH/sysvars.h>
#include <TH/die.h>
#include <TH/kernel.h>
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

#define PR_MAP "MAP: "
#if (0)
#define pr_map(...) pr_debug(PR_MAP __VA_ARGS__)
#else
#define pr_map(...)
#endif
/**
 * this is far more different from POSIX mmap
 * void *kmmap(void *addr, size_t length)
 * @addr: address, which should be mapped
 * @length: length of mmapped area
 */
LIKELY void *kmmap(void *addr, size_t length)
{
        /* used when addr not PG aligned */
        uintptr_t offset = 0;
        uintptr_t addr_old;

        pr_map("addr 0x%x, len 0x%x", addr, length);
        if(addr < last_addr()) {
                pr_map("not mapping it!");
                /* we are mapping kernel memory to kernel memory, 
                 * aren`t we?  */
                return addr;
        }

        /* align it manually!!! */
        if(!IS_ALIGNED((uintptr_t)addr, _2MBYTE)) {
                pr_map("Is not PG aligned");
                addr_old = (uintptr_t)addr;
                addr = (void*) ALIGN_DOWN((uintptr_t)addr, _2MBYTE);
                offset = addr_old - (uintptr_t) addr;
                pr_map("align addr down to 0x%x, offs 0x%x", addr, offset);
        }
        if(mmap_current >= MMAP_END) {
                pr_map("mem overflow");
                return NULL;
        }
        void* virt_addr = (void*)(uintptr_t)(_2MBYTE * mmap_current + offset);
        uint32_t mmap_old = mmap_current;
        length = _2MBYTE_ALIGN(length);
        pr_map("align len to 0x%x", length);
        size_t _2mbyte_length = length / _2MBYTE;
        while(_2mbyte_length-- > 0) {
                pr_map("to addr 0x%x", p2_table[mmap_current]);
                p2_table[mmap_current++] = (uintptr_t)addr|_MEM_AVAILABLE;
                addr += _2MBYTE;
                if(mmap_current >= MMAP_END) {
                        mmap_current = mmap_old;
                        return NULL;
                }
        }
        pr_map("initial addr is 0x%x", virt_addr);
        return virt_addr;
}


