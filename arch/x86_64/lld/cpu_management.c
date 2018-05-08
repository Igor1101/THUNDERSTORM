#include <kstdio.h>
void cpu_halt(void)
{
  kputs("HALTING");
  asm volatile (" halt: hlt\n"
      "jmp halt\n"
      );
}
