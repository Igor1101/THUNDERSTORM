#include <stdint.h>
#include <TH/stack.h>
#include <asm/traps.h>
#include <x86_64/idt.h>
#include <x86_64/tss.h>

#define PG_SIZE 4096
#define x64_LOW(X) ( (uint64_t) (X) & 0xFFFF )
#define x64_MIDDLE_LOW(X) ( (uint64_t) (X) >> 16 & 0xFF )
#define x64_MIDDLE_HIGH(X) ( (uint64_t) (X) >> 24 & 0xFF )
#define x64_HIGH(X) ( (uint64_t) (X) >> 32 )
#define HIGH(X) x64_HIGH(X)
#define LOW(X)  ( (uint64_t) (X) & 0xFFFFFFFF)

struct tss_format tss_table;


ALIGN(PG_SIZE) char exception_stk[SIZEOF_EXC_STACK];

void set_tss_desc(void)
{
        uint64_t base = (uint64_t)&tss_table;
        uint64_t limit = sizeof(struct tss_format) + base - 1;
        *GDT_tss_desc = (struct tss_desc){
                .low_limit      =       x64_LOW(limit),
                .base_15_0      =       x64_LOW(&tss_table),
                .base_23_16     =       x64_MIDDLE_LOW(&tss_table),
                .type           =       PRESENT|0b1001,
                .high_limit     =       x64_MIDDLE_LOW(limit),
                .base_31_24     =       x64_MIDDLE_HIGH(&tss_table),
                .base_63_32     =       x64_HIGH(&tss_table),
                .reserved       =       0
        };
}
void set_tss_table(void)
{
        tss_table = (struct tss_format){
                .reserved0      =       0,
                .rsp0_low       =       0,
                .rsp0_high      =       0,
                .rsp1_low       =       0,
                .rsp1_high      =       0,
                .rsp2_low       =       0,
                .rsp2_high      =       0,
                .reserved1      =       0,
                .reserved2      =       0,
                .ist1_low       =       LOW(EXCEPTION_STK_END),
                .ist1_high      =       HIGH(EXCEPTION_STK_END),
                .ist2_low       =       0,
                .ist2_high      =       0,
                .ist3_low       =       0,
                .ist3_high      =       0,
                .ist4_low       =       LOW(PAGE_FAULT_STK_END),
                .ist4_high      =       HIGH(PAGE_FAULT_STK_END),
                .ist5_low       =       LOW(NMI_STK_END),
                .ist5_high      =       HIGH(NMI_STK_END),
                .ist6_low       =       LOW(GENERAL_PROTECTION_STK_END),
                .ist6_high      =       HIGH(GENERAL_PROTECTION_STK_END),
                .ist7_low       =       LOW(DOUBLE_FAULT_STK_END),
                .ist7_high      =       HIGH(DOUBLE_FAULT_STK_END),
                .reserved3      =       0,
                .reserved4      =       0,
                .io_base        =       0,
                .reserved5      =       0
        };
}
