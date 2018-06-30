#include <gcc_opt.h>
#include <asm/traps.h>
#include <asm/int_handler.h>

INTERRUPT void divide_error(struct interrupt_frame *frame)
{
  EXC_START;
  kputs("divide by zero");
}
INTERRUPT void debug(struct interrupt_frame *frame){};
INTERRUPT void nmi(struct interrupt_frame *frame){};
/* breakpoint handling */
INTERRUPT void int3(struct interrupt_frame *frame){};
INTERRUPT void overflow(struct interrupt_frame *frame)
{
  EXC_START;
  kputs("overflow");
};
INTERRUPT void bounds(struct interrupt_frame *frame){};
INTERRUPT void invalid_op(struct interrupt_frame *frame)
{
  EXC_START;
  kputs("invalid opcode");
}

INTERRUPT void device_not_available(struct interrupt_frame *frame)
{
  EXC_START;
  kputs("device not available");
};
INTERRUPT void double_fault(struct interrupt_frame *frame, uword_t err){};
INTERRUPT void coprocessor_segment_overrun(struct interrupt_frame *frame){};
INTERRUPT void invalid_TSS(struct interrupt_frame *frame, uword_t err){};
INTERRUPT void segment_not_present(struct interrupt_frame *frame, uword_t err){};
INTERRUPT void stack_segment(struct interrupt_frame *frame, uword_t err){};
INTERRUPT void general_protection(struct interrupt_frame *frame, uword_t err){};
INTERRUPT void page_fault(struct interrupt_frame *frame, uword_t err){};
INTERRUPT void async_page_fault(struct interrupt_frame *frame){};
INTERRUPT void spurious_interrupt_bug(struct interrupt_frame *frame){};
INTERRUPT void coprocessor_error(struct interrupt_frame *frame){};
INTERRUPT void alignment_check(struct interrupt_frame *frame, uword_t err){};

INTERRUPT void undefined(struct interrupt_frame *frame){};

INTERRUPT void machine_check(struct interrupt_frame *frame){};
INTERRUPT void simd_coprocessor_error(struct interrupt_frame *frame){};

