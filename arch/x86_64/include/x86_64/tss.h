#ifndef TSS_H
#define TSS_H

#include <gcc_opt.h>
#include <stdint.h>

struct tss_format {
        uint32_t reserved0;
        uint32_t rsp0_low;
        uint32_t rsp0_high;
        uint32_t rsp1_low;
        uint32_t rsp1_high;
        uint32_t rsp2_low;
        uint32_t rsp2_high;
        uint32_t reserved1;
        uint32_t reserved2;
        uint32_t ist1_low;
        uint32_t ist1_high;
        uint32_t ist2_low;
        uint32_t ist2_high;
        uint32_t ist3_low;
        uint32_t ist3_high;
        uint32_t ist4_low;
        uint32_t ist4_high;
        uint32_t ist5_low;
        uint32_t ist5_high;
        uint32_t ist6_low;
        uint32_t ist6_high;
        uint32_t ist7_low;
        uint32_t ist7_high;
        uint32_t reserved3;
        uint32_t reserved4;
        uint16_t io_base;
        uint16_t reserved5;
} PACKED;

extern struct tss_format tss_table;

struct tss_desc {
        uint16_t low_limit;
        uint16_t base_15_0;
        uint8_t base_23_16;
        uint8_t type;
        uint8_t high_limit;
        uint8_t base_31_24;
        uint16_t base_63_32;
        uint32_t reserved;
} PACKED;

extern struct tss_desc* GDT_tss_desc;
void set_tss_desc(void);
void set_tss_table(void);

#endif /* TSS_H */
