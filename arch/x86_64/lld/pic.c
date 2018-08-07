#include <stdint.h>
#include <x86_64/cpu_management.h>
#include <x86_64/pic.h>
/**
 * void pic_remap(int int_offset1, int int_offset2)
 * reinitialize the PIC controllers, giving them specified vector offsets
 * rather than 8h and 70h, as configured by default 
 * @int_offset1:vector offset for master PIC 
 * vectors on the master become int_offset1..int_offset1+7 
 * @int_offset2: same for slave PIC: int_offset2..int_offset2+7
 */
void pic_remap(int int_offset1, int int_offset2)
{
	uint8_t a1, a2;
        /* save masks */
	a1 = inb(PIC1_IMR);      
	a2 = inb(PIC2_IMR);
 
        /* cascade mode */
        /* ICW1 */
	outb(PIC1_CMD, ICW1_INIT+ICW1_ICW4);  
	outb(PIC2_CMD, ICW1_INIT+ICW1_ICW4);
        /* ICW2: PIC vector offset */
	outb(PIC1_IMR, int_offset1);      
	outb(PIC2_IMR, int_offset2);
        /* ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100) */
	outb(PIC1_IMR, 4);        
        /* ICW3: tell Slave PIC its cascade identity (0000 0010) */
	outb(PIC2_IMR, 2); 
 
	outb(PIC1_IMR, ICW4_8086);
	outb(PIC2_IMR, ICW4_8086);
        /* restore saved masks. */
	outb(PIC1_IMR, a1);   
	outb(PIC2_IMR, a2);
}
