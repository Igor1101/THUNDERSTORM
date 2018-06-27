#include <stdint.h>
#include <kstring.h>
#define NUM_OF_EXCEPTIONS 32
#define NUM_OF_DESC 256
#define SIZE_OF_DESC  sizeof ( struct IDTdesc )
#define SIZE_OF_IDT NUM_OF_DESC * SIZE_OF_DESC
#define EARLY_SIZE_OF_IDT NUM_OF_EXCEPTIONS * SIZE_OF_DESC
struct IDTdesc 
{
   uint16_t offset_0_15; // offset bits 0..15
   uint16_t selector; // a code segment selector in GDT or LDT
   uint8_t ist;       /* bits 0..2 hold Interrupt Stack Table offset
                       *  , rest of bits - zero.*/
   uint8_t type_attr; // type and attributes
   uint16_t offset_16_31; // offset bits 16..31
   uint32_t offset_32_63; // offset bits 32..63
   uint32_t zero;     // reserved
} PACKED;
extern struct IDTdesc idt_table[NUM_OF_DESC];

extern void idt_set_vector(uint8_t num, uint64_t addr, 
    uint16_t sel, uint8_t attr, uint8_t ist);

FORCE_INLINE void idt_clear_vectors(void)
{
  kmemset_show(idt_table, 0, sizeof(idt_table));
}

FORCE_INLINE void idt_set_trap(uint8_t num, uint64_t addr,
    uint16_t sel, uint8_t ist)
{
  idt_set_vector(
      num, addr, sel, 0b1111, ist);
}
