#include <x86_64/idt.h>
#include <x86_64/tss.h>
#include <TH/stack.h>

#define x64_LOW(X) ( (uint64_t) (X) & 0xFFFF )
#define x64_MIDDLE_LOW(X) ( (uint64_t) (X) >> 16 & 0xFF )
#define x64_MIDDLE_HIGH(X) ( (uint64_t) (X) >> 24 & 0xFF )
#define x64_HIGH(X) ( (uint64_t) (X) >> 32 )
#define HIGH(X) x64_HIGH(X)
#define LOW(X)  ( (uint64_t) (X) & 0xFFFFFFFF)
struct tss_format tss_table;

void set_tss_desc(void)
{
        *GDT_tss_desc = (struct tss_desc){
                .low_limit      =       0xFFFF,
                .base_15_0      =       x64_LOW(&tss_table),
                .base_23_16     =       x64_MIDDLE_LOW(&tss_table),
                .type           =       PRESENT|0b1001,
                .high_limit     =       0xFF,
                .base_31_24     =       x64_MIDDLE_HIGH(&tss_table),
                .base_63_32     =       x64_HIGH(&tss_table),
                .reserved       =       0
        };
}
void set_tss_table(void)
{
        tss_table = (struct tss_format){
                .reserved0      =       0,
                .rsp0_low       =       LOW(&exc_stack_top),
                .rsp0_high      =       HIGH(&exc_stack_top),
                .rsp1_low       =       LOW(&exc_stack_top),
                .rsp1_high      =       HIGH(&exc_stack_top),
                .rsp2_low       =       LOW(&exc_stack_top),
                .rsp2_high      =       HIGH(&exc_stack_top),
                .reserved1      =       0,
                .reserved2      =       0,
                .ist1_low       =       LOW(&idt_table),
                .ist1_high      =       HIGH(&idt_table),
                .ist2_low       =       LOW(&idt_table),
                .ist2_high      =       HIGH(&idt_table),
                .ist3_low       =       LOW(&idt_table),
                .ist3_high      =       HIGH(&idt_table),
                .ist4_low       =       LOW(&idt_table),
                .ist4_high      =       HIGH(&idt_table),
                .ist5_low       =       LOW(&idt_table),
                .ist5_high      =       HIGH(&idt_table),
                .ist6_low       =       LOW(&idt_table),
                .ist6_high      =       HIGH(&idt_table),
                .ist7_low       =       LOW(&idt_table),
                .ist7_high      =       HIGH(&idt_table),
                .reserved3      =       0,
                .reserved4      =       0,
                .io_base        =       0,
                .reserved5      =       0
        };
}
