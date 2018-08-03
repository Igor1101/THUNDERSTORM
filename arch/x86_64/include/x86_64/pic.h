#ifndef PIC_H
#define PIC_H
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
