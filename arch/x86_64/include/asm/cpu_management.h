#ifndef CPU_HL_MANAGMENT
#define CPU_HL_MANAGMENT
/*
 * Copyright (C) 2018-2019  Igor Muravyov <igor.muravyov.2015@gmail.com>
 */

#include <TH/kernel.h>

FORCE_INLINE void local_irq_enable(void)
{                               /* set external interrupts */
        asm volatile (" sti ");
}

FORCE_INLINE void local_irq_disable(void)
{                               /* clear external interrupts */
        asm volatile (" cli ");
}

NORET FORCE_INLINE void cpu_halt(void)
{
        pr_notice("HALTING");
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
