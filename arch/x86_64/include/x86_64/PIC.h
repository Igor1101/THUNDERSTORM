/* THIS FILE IS MARKED AS PUBLIC DOMAIN */
#ifndef PIC_H
#define PIC_H

#include <stdint.h>
#include <gcc_opt.h>
#include <x86_64/cpu_management.h>
#include <x86_64/IDT.h>

#define PIC1		0x20
#define PIC2		0xA0
#define PIC1_CMD	PIC1
#define PIC2_CMD	PIC2
/* Interrupt mask registers */
#define PIC1_IMR	(PIC1+1)
#define PIC2_IMR	(PIC2+1)
/* end of interrupt */
#define PIC_EOI         0x20
#define ICW1_ICW4	0x01		/* ICW4 (not) needed */
#define ICW1_SINGLE	0x02		/* Single (cascade) mode */
#define ICW1_INTERVAL4	0x04		/* Call address interval 4 (8) */
#define ICW1_LEVEL	0x08		/* Level triggered (edge) mode */
#define ICW1_INIT	0x10		/* Initialization - required! */
 
#define ICW4_8086	0x01		/* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO	0x02		/* Auto (normal) EOI */
#define ICW4_BUF_SLAVE	0x08		/* Buffered mode/slave */
#define ICW4_BUF_MASTER	0x0C		/* Buffered mode/master */
#define ICW4_SFNM	0x10		/* Special fully nested (not) */

/* INTS mapping */
#define IRQ0_OFFSET (NUM_OF_EXCEPTIONS - 1)
#define IRQ1_OFFSET (NUM_OF_EXCEPTIONS - 1 + 8)
 
/**
 * void pic_disable(void)
 * Mask master/slave Interrupt mask register,
 * */
FORCE_INLINE void pic_disable(void)
{
        outb(PIC2_IMR, 0xFF);
        outb(PIC1_IMR, 0xFF);
}

FORCE_INLINE void pic_sendEOI(uint8_t irq)
{
	if(irq >= 8)
		outb(PIC2_CMD,PIC_EOI);
	outb(PIC1_CMD,PIC_EOI);
}

FORCE_INLINE void pic_set_mask(uint8_t IRQline) 
{
        uint16_t port;
        uint8_t value;
 
        if(IRQline < 8) {
                port = PIC1_IMR;
        } else {
                port = PIC2_IMR;
                IRQline -= 8;
        }
        value = inb(port) | (1 << IRQline);
        outb(port, value);        
}
 
FORCE_INLINE void pic_clear_mask(uint8_t IRQline) 
{
        uint16_t port;
        uint8_t value;
 
        if(IRQline < 8) {
                port = PIC1_IMR;
        } else {
                port = PIC2_IMR;
                IRQline -= 8;
        }
        value = inb(port) & ~(1 << IRQline);
        outb(port, value);        
}

/**
 * void pic_remap(int int_offset1, int int_offset2)
 * reinitialize the PIC controllers, giving them specified vector offsets
 * rather than 8h and 70h, as configured by default 
 * @offset1:vector offset for master PIC 
 * vectors on the master become offset1..offset1+7 
 * @offset2: same for slave PIC: offset2..offset2+7
 */
UNLIKELY void pic_remap(int int_offset1, int int_offset2);

#endif /* PIC_H */
