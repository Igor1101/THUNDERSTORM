#include <x86_64/idt.h>
#include <x86_64/cpu_management.h>

UNLIKELY void init_interrupts(void)
{
  lidt(idt_table, EARLY_SIZE_OF_IDT );
}
