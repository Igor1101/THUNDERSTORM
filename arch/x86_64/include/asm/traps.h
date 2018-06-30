/* SPDX-License-Identifier: GPL-2.0 */
/* copied from Linux kernel 4.18.0-rc1-ARCH+,
 * modified by Igor Muravyov <igor.muravyov.2015@gmail.com>
 * for THUNDERSTORM kernel
 * asmlikage useless here*/
#ifndef _ASM_X86_TRAPS_H
#define _ASM_X86_TRAPS_H

#include <gcc_opt.h>
#include <x86_64/idt.h>
#define CONFIG_X86_64
#define dotraplinkage //looks like do_* functions are useless here
/* exceptions */
INTERRUPT void divide_error(struct interrupt_frame *frame);
INTERRUPT void debug(struct interrupt_frame *frame);
INTERRUPT void nmi(struct interrupt_frame *frame);
/* breakpoint handling */
INTERRUPT void int3(struct interrupt_frame *frame);
INTERRUPT void overflow(struct interrupt_frame *frame);
INTERRUPT void bounds(struct interrupt_frame *frame);
INTERRUPT void invalid_op(struct interrupt_frame *frame);
INTERRUPT void device_not_available(struct interrupt_frame *frame);
#ifdef CONFIG_X86_64
INTERRUPT void double_fault(struct interrupt_frame *frame, uword_t err);
#endif
INTERRUPT void coprocessor_segment_overrun(struct interrupt_frame *frame);
INTERRUPT void invalid_TSS(struct interrupt_frame *frame, uword_t err);
INTERRUPT void segment_not_present(struct interrupt_frame *frame, uword_t err);
INTERRUPT void stack_segment(struct interrupt_frame *frame, uword_t err);
INTERRUPT void general_protection(struct interrupt_frame *frame, uword_t err);
INTERRUPT void page_fault(struct interrupt_frame *frame, uword_t err);
INTERRUPT void async_page_fault(struct interrupt_frame *frame);
INTERRUPT void spurious_interrupt_bug(struct interrupt_frame *frame);
INTERRUPT void coprocessor_error(struct interrupt_frame *frame);
INTERRUPT void alignment_check(struct interrupt_frame *frame, uword_t err);

INTERRUPT void undefined(struct interrupt_frame *frame);

INTERRUPT void machine_check(struct interrupt_frame *frame);
INTERRUPT void simd_coprocessor_error(struct interrupt_frame *frame);

#if defined(CONFIG_X86_64) && defined(CONFIG_XEN_PV)
INTERRUPT void xen_divide_error(struct interrupt_frame *frame);
INTERRUPT void xen_xennmi(struct interrupt_frame *frame);
INTERRUPT void xen_xendebug(struct interrupt_frame *frame);
INTERRUPT void xen_xenint3(struct interrupt_frame *frame);
INTERRUPT void xen_overflow(struct interrupt_frame *frame);
INTERRUPT void xen_bounds(struct interrupt_frame *frame);
INTERRUPT void xen_invalid_op(struct interrupt_frame *frame);
INTERRUPT void xen_device_not_available(struct interrupt_frame *frame);
INTERRUPT void xen_double_fault(struct interrupt_frame *frame);
INTERRUPT void xen_coprocessor_segment_overrun(struct interrupt_frame *frame);
INTERRUPT void xen_invalid_TSS(struct interrupt_frame *frame);
INTERRUPT void xen_segment_not_present(struct interrupt_frame *frame);
INTERRUPT void xen_stack_segment(struct interrupt_frame *frame);
INTERRUPT void xen_general_protection(struct interrupt_frame *frame);
INTERRUPT void xen_page_fault(struct interrupt_frame *frame);
INTERRUPT void xen_spurious_interrupt_bug(struct interrupt_frame *frame);
INTERRUPT void xen_coprocessor_error(struct interrupt_frame *frame);
INTERRUPT void xen_alignment_check(struct interrupt_frame *frame);
#ifdef CONFIG_X86_MCE
INTERRUPT void xen_machine_check(struct interrupt_frame *frame);
#endif /* CONFIG_X86_MCE */
INTERRUPT void xen_simd_coprocessor_error(struct interrupt_frame *frame);
#endif
/*
dotraplinkage void do_divide_error(struct pt_regs *, long);
dotraplinkage void do_debug(struct pt_regs *, long);
dotraplinkage void do_nmi(struct pt_regs *, long);
dotraplinkage void do_int3(struct pt_regs *, long);
dotraplinkage void do_overflow(struct pt_regs *, long);
dotraplinkage void do_bounds(struct pt_regs *, long);
dotraplinkage void do_invalid_op(struct pt_regs *, long);
dotraplinkage void do_device_not_available(struct pt_regs *, long);
dotraplinkage void do_coprocessor_segment_overrun(struct pt_regs *, long);
dotraplinkage void do_invalid_TSS(struct pt_regs *, long);
dotraplinkage void do_segment_not_present(struct pt_regs *, long);
dotraplinkage void do_stack_segment(struct pt_regs *, long);
#ifdef CONFIG_X86_64
dotraplinkage void do_double_fault(struct pt_regs *, long);
#endif
dotraplinkage void do_general_protection(struct pt_regs *, long);
dotraplinkage void do_page_fault(struct pt_regs *, unsigned long);
dotraplinkage void do_spurious_interrupt_bug(struct pt_regs *, long);
dotraplinkage void do_coprocessor_error(struct pt_regs *, long);
dotraplinkage void do_alignment_check(struct pt_regs *, long);
#ifdef CONFIG_X86_MCE
dotraplinkage void do_machine_check(struct pt_regs *, long);
#endif
dotraplinkage void do_simd_coprocessor_error(struct pt_regs *, long);
#ifdef CONFIG_X86_32
dotraplinkage void do_iret_error(struct pt_regs *, long);
#endif
dotraplinkage void do_mce(struct pt_regs *, long);
*/
extern void (* exceptions_array[]);












/*
static inline int get_si_code(unsigned long condition)
{
	if (condition & DR_STEP)
		return TRAP_TRACE;
	else if (condition & (DR_TRAP0|DR_TRAP1|DR_TRAP2|DR_TRAP3))
		return TRAP_HWBKPT;
	else
		return TRAP_BRKPT;
}

extern int panic_on_unrecovered_nmi;

void math_emulate(struct math_emu_info *);
#ifndef CONFIG_X86_32
INTERRUPT void smp_thermal_interrupt(struct interrupt_frame *frame);
INTERRUPT void smp_threshold_interrupt(struct interrupt_frame *frame);
INTERRUPT void smp_deferred_error_interrupt(struct interrupt_frame *frame);
#endif

extern void ist_enter(void);
extern void ist_exit(void);
extern void ist_begin_non_atomic(void);
extern void ist_end_non_atomic(void);

#ifdef CONFIG_VMAP_STACK
void __noreturn handle_stack_overflow(const char *message,
				      struct pt_regs *regs,
				      unsigned long fault_address);
#endif
*/
/* Interrupts/Exceptions */
enum {
	X86_TRAP_DE = 0,	/*  0, Divide-by-zero */
	X86_TRAP_DB,		/*  1, Debug */
	X86_TRAP_NMI,		/*  2, Non-maskable Interrupt */
	X86_TRAP_BP,		/*  3, Breakpoint */
	X86_TRAP_OF,		/*  4, Overflow */
	X86_TRAP_BR,		/*  5, Bound Range Exceeded */
	X86_TRAP_UD,		/*  6, Invalid Opcode */
	X86_TRAP_NM,		/*  7, Device Not Available */
	X86_TRAP_DF,		/*  8, Double Fault */
	X86_TRAP_OLD_MF,	/*  9, Coprocessor Segment Overrun */
	X86_TRAP_TS,		/* 10, Invalid TSS */
	X86_TRAP_NP,		/* 11, Segment Not Present */
	X86_TRAP_SS,		/* 12, Stack Segment Fault */
	X86_TRAP_GP,		/* 13, General Protection Fault */
	X86_TRAP_PF,		/* 14, Page Fault */
	X86_TRAP_SPURIOUS,	/* 15, Spurious Interrupt */
	X86_TRAP_MF,		/* 16, x87 Floating-Point Exception */
	X86_TRAP_AC,		/* 17, Alignment Check */
	X86_TRAP_MC,		/* 18, Machine Check */
	X86_TRAP_XF,		/* 19, SIMD Floating-Point Exception */
	X86_TRAP_IRET = 32,	/* 32, IRET Exception */
};

/*
 * Page fault error code bits:
 *
 *   bit 0 ==	 0: no page found	1: protection fault
 *   bit 1 ==	 0: read access		1: write access
 *   bit 2 ==	 0: kernel-mode access	1: user-mode access
 *   bit 3 ==				1: use of reserved bit detected
 *   bit 4 ==				1: fault was an instruction fetch
 *   bit 5 ==				1: protection keys block access
 */
enum x86_pf_error_code {
	X86_PF_PROT	=		1 << 0,
	X86_PF_WRITE	=		1 << 1,
	X86_PF_USER	=		1 << 2,
	X86_PF_RSVD	=		1 << 3,
	X86_PF_INSTR	=		1 << 4,
	X86_PF_PK	=		1 << 5,
};
#endif /* _ASM_X86_TRAPS_H */
