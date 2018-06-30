#ifndef CPU_HL_MANAGMENT
#define CPU_HL_MANAGMENT
#include <kstdio.h>
FORCE_INLINE void set_interrupts(void)
{/* set external interrupts */
  asm volatile(" sti ");
}

FORCE_INLINE void clear_interrupts(void)
{/* clear external interrupts */
  asm volatile(" cli ");
}

NORET FORCE_INLINE void cpu_halt(void)
{
  kputs("HALTING");
  asm volatile (
                " halt: hlt\n"
                " jmp   halt\n"
      );
  while(1);
}

#endif
