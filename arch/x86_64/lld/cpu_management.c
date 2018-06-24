#include <kstdio.h>
#include <gcc_opt.h>
UNLIKELY void cpu_halt(void)
{
  kputs("HALTING");
  asm volatile (" halt: hlt\n"
      "jmp halt\n"
      );
}
