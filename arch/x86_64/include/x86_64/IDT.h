/*
 * Copyright (C) 2018  Igor Muravyov <igor.muravyov.2015@gmail.com>
 */

#ifndef x86_64_IDT_H
#define x86_64_IDT_H

#include <stdint.h>
#include <kstring.h>
#include <kstdio.h>
#include <TH/kernel.h>
#include <TH/sysvars.h>
#include <x86_64/cpu_management.h>
#define NUM_OF_EXCEPTIONS 32
#define NUM_OF_DESC 256
#define SIZE_OF_DESC  sizeof ( struct IDTdesc )
#define SIZE_OF_IDT NUM_OF_DESC * SIZE_OF_DESC
#define EARLY_SIZE_OF_IDT NUM_OF_EXCEPTIONS * SIZE_OF_DESC
#define PRESENT         (1<<7)
#define TRAP            (0b1111)
#define INTERR          (0b1110)
#define CALL_GATE       (0b1100)
#define TSS_AVAILABLE   (0b1001)
#define TSS_BUSY        (0b1011)
#define LDT             (0b0010)

typedef unsigned long long int uword_t;

struct interrupt_frame {
        uword_t rip;
        uword_t cs;
        uword_t rflags;
        uword_t rsp;
        uword_t ss;
} PACKED;
typedef struct interrupt_frame int_frame;

struct IDTdesc {
        uint16_t offset_0_15;   // offset bits 0..15
        uint16_t selector;      // a code segment selector in GDT or LDT
        uint8_t ist;            /* bits 0..2 hold Interrupt Stack Table offset
                                 *  , rest of bits - zero.*/
        uint8_t type_attr;      // type and attributes
        uint16_t offset_16_31;  // offset bits 16..31
        uint32_t offset_32_63;  // offset bits 32..63
        uint32_t zero;          // reserved
} PACKED;
extern struct IDTdesc idt_table[NUM_OF_DESC];

extern void idt_set_vector(uint8_t num, uint64_t addr,
                           uint16_t sel, uint8_t attr, uint8_t ist);

FORCE_INLINE void idt_clear_vectors(void)
{
        pr_notice("clearing vectors:");
        kmemset_show(idt_table, 0, sizeof(idt_table));
        text.putchar('\n');
}

FORCE_INLINE void idt_set_trap(uint8_t num, uint64_t addr, uint8_t ist)
{
        idt_set_vector(num, addr, get_CS(), TRAP | PRESENT, ist);
}

#endif                          /* x86_64_IDT_H */
