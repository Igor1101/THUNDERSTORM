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

#endif                          /* CPU_HL_MANAGMENT */
