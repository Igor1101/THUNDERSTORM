#include <stdint.h>
#include <gcc_opt.h>
#include <TH/lld.h>
#include <x86_64/idt.h>
#include <asm/cpu_management.h>
#define x64_LOW(X) ( (uint64_t) (X) & 0xFFFF )
#define x64_MIDDLE(X) ( (uint64_t) (X) >> 16 & 0xFFFF )
#define x64_HIGH(X) ( (uint64_t) (X) >> 32 )
ALIGN (8) struct IDTdesc idt_table[NUM_OF_DESC];
UNLIKELY void 
idt_set_vector(uint8_t num, uint64_t addr, 
    uint16_t sel, uint8_t type_attr, uint8_t ist)
{
  idt_table[num] = (struct IDTdesc)
  {
    .offset_0_15  = x64_LOW(addr),
    .selector     = sel,
    .ist          = ist,
    .type_attr    = type_attr,
    .offset_16_31 = x64_MIDDLE(addr),
    .offset_32_63 = x64_HIGH(addr),
    .zero         = 0
  };
}


INTERRUPT void divide_byzero(struct interrupt_frame *frame, uword_t error_code)
{
  kprintf("rsp: %d, \nss: %d, \nrflags: %d, \ncs: %d, \nrip %d", 
      frame -> rsp,
      frame -> ss,
      frame -> rflags,
      frame -> cs,
      frame -> rip
      );
  cpu_halt();
}

UNLIKELY void set_exceptions(void)
{
  idt_clear_vectors();
  for(uint32_t num = 0; num < 32; num++)
  {
    idt_set_trap(num, (uint64_t)&divide_byzero, 0);
  }
}

