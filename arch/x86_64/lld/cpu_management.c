#include <kstdio.h>
void cpu_halt(void)
{
  kputs("HALTING");
  asm volatile ("hlt");
}
