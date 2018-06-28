#include <kstdio.h>
FORCE_INLINE void set_interrupts(void)
{/* set external interrupts */
  asm volatile(" sti ");
}

FORCE_INLINE void clear_interrupts(void)
{/* clear external interrupts */
  asm volatile(" cli ");
}

FORCE_INLINE void cpu_halt(void)
{
  kputs("HALTING");
  asm volatile (
                " halt: hlt\n"
                " jmp   halt\n"
      );
}
