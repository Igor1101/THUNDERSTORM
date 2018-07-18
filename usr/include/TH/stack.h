#ifndef STACK_H
#define STACK_H

#include <gcc_opt.h>
#include <stdint.h>
#include <kstring.h>

#ifndef SIZEOF_EXC_STACK
#define SIZEOF_EXC_STACK 1024 * 8
#endif /* SIZEOF_EXC_STACK */

#define EXCEPTION_STK_END \
        ((uintptr_t)&exception_stk + sizeof(exception_stk) - sizeof(uintptr_t))
extern char exception_stk[SIZEOF_EXC_STACK];

extern void* stack_bottom;/*  Main initialization stack */
extern void* stack_top;

FORCE_INLINE void clear_kernel_stacks(void)
{
        kmemset_show(&exception_stk, 0, sizeof (exception_stk));
}

#endif
