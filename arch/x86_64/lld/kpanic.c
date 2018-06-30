#include <gcc_opt.h>
#include <kstdio.h>
#include <misc/convert.h>
#include <TH/lld.h>
#include <asm/kpanic.h>
#include <asm/cpu_management.h>
#include <x86_64/idt.h>


/*
 * TAB=2 SPACES
 * After exception\ int occured we have in stack:
 * SS                         -8
 * RSP                        -16
 * RFLAGS                     -24
 * CS                         -32
 * RIP                        -40
 * ERRCODE(optional)          -48
 * R11  |---> pushed by gcc
 * R10  |
 * R9   |
 * R8   |
 * RDI  |
 * RSI  |
 * RCX  |                     
 * RDX  |                     
 * RAX  | 
 */
UNLIKELY NORET void kpanic(char*reason, int_frame* fr)
{
  select_bgcolor(Red);
  kprintf("\nKERNEL PANIC:");
  select_bgcolor(DefaultBG);
  kprintf("reason: %s\n", reason);
  kprintf("rip=0x%x,\ncs=0x%x,\nss=0x%x,\nrflags=0x%x,\nrsp=0x%x\n",
      fr -> rip,
      fr -> cs,
      fr -> ss,
      fr -> rflags,
      fr -> rsp
      );
  cpu_halt();
}
