/* Copyright (C) 2018  Igor Muravyov <igor.muravyov.2015@gmail.com> */
#include <gcc_opt.h>
#include <asm/traps.h>
#include <asm/int_handler.h>
#include <asm/kpanic.h>
#include <x86_64/cpu_management.h>

INTERRUPT void divide_error(int_frame *frame)
{
  EXC_START;
  kputs("divide by zero");
  catch_regs(frame, (void*)get_SP());
  kpanic("Exception \"divide by zero\"");
}
INTERRUPT void debug(int_frame *frame){};
INTERRUPT void nmi(int_frame *frame){};
/* breakpoint handling */
INTERRUPT void int3(int_frame *frame){};
INTERRUPT void overflow(int_frame *frame)
{
  EXC_START;
  kputs("overflow");
};
INTERRUPT void bounds(int_frame *frame){};
INTERRUPT void invalid_op(int_frame *frame)
{
  EXC_START;
  kputs("invalid opcode");
  catch_regs(frame, (void*)get_SP());
  kpanic("Exception \"invalid opcode\"");
  EXC_EXIT;
}

INTERRUPT void device_not_available(int_frame *frame)
{
  EXC_START;
  kputs("device not available");
};
INTERRUPT void double_fault(int_frame *frame)
{
  EXC_START;
  kprintf("SYSTEM CRASH");
  catch_regs(frame, (void*)get_SP());
  kpanic("Exception \"double fault\"");
};
INTERRUPT void coprocessor_segment_overrun(int_frame *frame){};
INTERRUPT void invalid_TSS(int_frame *frame, uword_t err){};
INTERRUPT void segment_not_present(int_frame *frame, uword_t err){};
INTERRUPT void stack_segment(int_frame *frame, uword_t err){};
INTERRUPT void general_protection(int_frame *frame, uword_t err){};
INTERRUPT void page_fault(int_frame *frame, uword_t err){};
INTERRUPT void async_page_fault(int_frame *frame){};
INTERRUPT void spurious_interrupt_bug(int_frame *frame){};
INTERRUPT void coprocessor_error(int_frame *frame){};
INTERRUPT void alignment_check(int_frame *frame, uword_t err){};

INTERRUPT void undefined(int_frame *frame){};

INTERRUPT void machine_check(int_frame *frame){};
INTERRUPT void simd_coprocessor_error(int_frame *frame){};

