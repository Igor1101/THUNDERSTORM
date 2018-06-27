#include <stdint.h>
#include <gcc_opt.h>
#include <x86_64/idt.h>
#define x64_LOW(X) ( (uint64_t) (X) & 0xFFFF )
#define x64_MIDDLE(X) ( (uint64_t) (X) >> 16 & 0xFFFF )
#define x64_HIGH(X) ( (uint64_t) (X) >> 32 )
struct IDTdesc idt_table[NUM_OF_DESC];
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
<<<<<<< HEAD
#ifdef __x86_64__
typedef unsigned long long int uword_t;
#else
typedef unsigned int uword_t;
#endif

struct interrupt_frame;

INTERRUPT void divide_byzero(struct interrupt_frame *frame, uword_t error_code)
{

}
UNLIKELY void set_exceptions(void)
{
  idt_set_trap(0, (uint64_t)&divide_byzero, 0);
}
=======

>>>>>>> fbc8d093eb777ef79fbe6b6d86d5d2412ae235f2

