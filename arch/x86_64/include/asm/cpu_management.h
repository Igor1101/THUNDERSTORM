#ifndef CPU_HL_MANAGMENT
#define CPU_HL_MANAGMENT
/*
 * Copyright (C) 2018  Igor Muravyov <igor.muravyov.2015@gmail.com>
 */
#include <kstdio.h>

FORCE_INLINE void set_interrupts(void)
{                               /* set external interrupts */
        asm volatile (" sti ");
}

FORCE_INLINE void clear_interrupts(void)
{                               /* clear external interrupts */
        asm volatile (" cli ");
}

NORET FORCE_INLINE void cpu_halt(void)
{
        kputs("HALTING");
        while (1)
                asm volatile ("hlt");
}

/*
 * The use of this function in portable 
 * (HL) source is appropriate
 */
FORCE_INLINE uint64_t get_SP(void)
{
        uint64_t rsp;
        asm("mov %%rsp, %0":"=r"(rsp));
        return rsp;
}

/* this function detects and prints cpuinfo */
int detect_cpu(void);
#endif                          /* CPU_HL_MANAGMENT */
