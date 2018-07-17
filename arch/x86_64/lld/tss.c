#include <x86_64/idt.h>
#include <x86_64/tss.h>

#define x64_LOW(X) ( (uint64_t) (X) & 0xFFFF )
#define x64_MIDDLE_LOW(X) ( (uint64_t) (X) >> 16 & 0xFF )
#define x64_MIDDLE_HIGH(X) ( (uint64_t) (X) >> 24 & 0xFF )
#define x64_HIGH(X) ( (uint64_t) (X) >> 32 )

struct tss_format tss_table;
void set_tss_desc(void)
{
        *GDT_tss_desc = (struct tss_desc){
                .low_limit      =0xFFFF,
                .base_15_0      =x64_LOW(&tss_table),
                .base_23_16     =x64_MIDDLE_LOW(&tss_table),
                .type           =PRESENT|0b1001,
                .high_limit     =0xFF,
                .base_31_24     =x64_MIDDLE_HIGH(&tss_table),
                .base_63_32     =x64_HIGH(&tss_table),
                .reserved       =0
        };
}
