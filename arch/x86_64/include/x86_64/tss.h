#include <stdint.h>

struct tss_format {
        uint32_t reserved;
        uint32_t rsp0_low;
        uint32_t rsp0_high;
        uint32_t rsp1_low;
        uint32_t rsp1_high;
        uint32_t rsp2_low;
        uint32_t rsp2_high;
        uint32_t reserved;
        uint32_t reserved;
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
        uint32_t reserved;
        uint32_t reserved;
        uint16_t base;
        uint16_t reserved;
} PACKED;
extern struct tss_format tss_table;
