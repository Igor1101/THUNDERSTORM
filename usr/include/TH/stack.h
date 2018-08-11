#ifndef STACK_H
#define STACK_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * TH static memory stacks
 */
#include <compiler_opt.h>
#include <stdint.h>
#include <kstring.h>

/* 
 * Sizeof stack MUST be multiple of 64
 * so we have 8 stacks in exception_stk array each
 * with size SIZEOF_EXC_STACK / 8
 * */
#ifndef SIZEOF_EXC_STACK
#define SIZEOF_EXC_STACK 1024 * 4 * 8
#endif /* SIZEOF_EXC_STACK */

#define EXCEPTION_STK_END \
        ((uintptr_t)&exception_stk + sizeof(exception_stk) - sizeof(uintptr_t))

#define SIZEOF_STK sizeof(exception_stk) / 8

#define DOUBLE_FAULT_STK_END ((uintptr_t)&exception_stk + SIZEOF_STK)
#define GENERAL_PROTECTION_STK_END ((uintptr_t)&exception_stk + SIZEOF_STK * 2)
#define NMI_STK_END ((uintptr_t)&exception_stk + SIZEOF_STK * 3)
#define PAGE_FAULT_STK_END ((uintptr_t)&exception_stk + SIZEOF_STK * 4)
//#define DOUBLE_FAULT_STK_END ((uintptr_t)&exception_stk + SIZEOF_STK * 5)
//#define DOUBLE_FAULT_STK_END ((uintptr_t)&exception_stk + SIZEOF_STK * 6)
#define EXCEPTION_STK_BEGINNIG ((uintptr_t)&exception_stk + SIZEOF_STK * 7)

extern char exception_stk[SIZEOF_EXC_STACK];

extern void* stack_bottom;/*  Main initialization stack */
extern void* stack_top;

FORCE_INLINE void clear_kernel_stacks(void)
{
        kmemset_show(&exception_stk, 0, sizeof (exception_stk));
}

#ifdef __cplusplus
}
#endif

#endif /* STACK_H */
