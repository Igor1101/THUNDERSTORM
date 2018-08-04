#ifndef PIC_H
#define PIC_H

#define PIC1		0x20		/* IO base address for master PIC */
#define PIC2		0xA0		/* IO base address for slave PIC */
#define PIC1_COMMAND	PIC1
#define PIC1_DATA	(PIC1+1)
#define PIC2_COMMAND	PIC2
#define PIC2_DATA	(PIC2+1)

/* THIS FILE IS MARKED AS PUBLIC DOMAIN */
FORCE_INLINE void pic_disable(void)
{
        __asm__ __volatile__ (
                "  mov  $0xff, %%al\n"
                "  out  %%al, $0xa1\n"
                "  out  %%al, $0x21\n"
                :
                :
                :"memory", "esi", "edi", "eax", "ebx", "ecx", "edx"
                );
}

#endif /* PIC_H */
