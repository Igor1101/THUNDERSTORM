/* Copyright (C) 2018  Igor Muravyov <igor.muravyov.2015@gmail.com> */
#include <gcc_opt.h>
#include <asm/traps.h>
#include <asm/int_handler.h>
#include <asm/kpanic.h>
#include <x86_64/cpu_management.h>

char *exception_messages[] = {
        "Division By Zero",
        "Debug",
          "Non Maskable Interrupt",
        "Breakpoint",
        "Into Detected Overflow",
         "Out of Bounds",
        "Invalid Opcode",
        "No Coprocessor",

        "Double Fault",
        "Coprocessor Segment Overrun",
        "Bad TSS",
        "Segment Not Present",
        "Stack Fault",
        "General Protection Fault", 
        "Page Fault",
        "Unknown Interrupt",

        "Coprocessor Fault",
        "Alignment Check",
        "Machine Check",
        "SIMD COPROCESSOR ERROR",
        "Unknown Interrupt",
        "Unknown Interrupt",
        "Unknown Interrupt",
        "Unknown Interrupt",
        "Unknown Interrupt",
        "Unknown Interrupt",
        "Unknown Interrupt",
        "Unknown Interrupt",
        "Unknown Interrupt",
        "Unknown Interrupt",
        "Unknown Interrupt",
        "Unknown Interrupt"
};

INTERRUPT void verify_handler(int_frame * frame)
{
        catch_regs(frame, (void *)get_SP());
}
INTERRUPT void divide_error(int_frame * frame)
{
        EXC_START;
        catch_regs(frame, (void *)get_SP());
        kpanic(exception_messages[DIVIDE_ERROR]);
}

INTERRUPT void debug(int_frame * frame)
{
        EXC_START;
        catch_regs(frame, (void *)get_SP());
        kpanic(exception_messages[DEBUG]);
};

INTERRUPT void nmi(int_frame * frame)
{
        EXC_START;
        catch_regs(frame, (void *)get_SP());
        kpanic(exception_messages[NMI]);
};

/* breakpoint handling */
INTERRUPT void int3(int_frame * frame)
{
        EXC_START;
        catch_regs(frame, (void *)get_SP());
        kpanic(exception_messages[INT3]);
};

INTERRUPT void overflow(int_frame * frame)
{
        EXC_START;
        catch_regs(frame, (void *)get_SP());
        kpanic(exception_messages[OVERFLOW]);
};

INTERRUPT void bounds(int_frame * frame)
{
        EXC_START;
        catch_regs(frame, (void *)get_SP());
        kpanic(exception_messages[BOUNDS]);
};

INTERRUPT void invalid_op(int_frame * frame)
{
        EXC_START;
        catch_regs(frame, (void *)get_SP());
        kpanic(exception_messages[INVALID_OP]);
}

INTERRUPT void device_not_available(int_frame * frame)
{
        EXC_START;
        catch_regs(frame, (void *)get_SP());
        kpanic(exception_messages[DEVICE_NOT_AVAILABLE]);
};

INTERRUPT void double_fault(int_frame * frame)
{
        EXC_START;
        catch_regs(frame, (void *)get_SP());
        kpanic(exception_messages[DOUBLE_FAULT]);
};

INTERRUPT void coprocessor_segment_overrun(int_frame * frame)
{
        EXC_START;
        catch_regs(frame, (void *)get_SP());
        kpanic(exception_messages[COPROCESSOR_SO]);
};

INTERRUPT void invalid_TSS(int_frame * frame, uword_t err)
{
        EXC_START;
        kprintf("ERROR CODE: 0x%x", err);
        catch_regs(frame, (void *)get_SP());
        kpanic(exception_messages[INVALID_TSS]);
};

INTERRUPT void segment_not_present(int_frame * frame, uword_t err)
{
        EXC_START;
        kprintf("ERROR CODE: 0x%x", err);
        catch_regs(frame, (void *)get_SP());
        kpanic(exception_messages[SEGMENT_NOT_PRESENT]);
};

INTERRUPT void stack_segment(int_frame * frame, uword_t err)
{
        EXC_START;
        kprintf("ERROR CODE: 0x%x", err);
        catch_regs(frame, (void *)get_SP());
        kpanic(exception_messages[STACK_SEGMENT]);
};

INTERRUPT void general_protection(int_frame * frame, uword_t err)
{
        EXC_START;
        kprintf("ERROR CODE: 0x%x", err);
        catch_regs(frame, (void *)get_SP());
        kpanic(exception_messages[GENERAL_PROTECTION]);
};

INTERRUPT void page_fault(int_frame * frame, uword_t err)
{
        EXC_START;
        kprintf("ERROR CODE: 0x%x", err);
        catch_regs(frame, (void *)get_SP());
        kpanic(exception_messages[PAGE_FAULT]);
};

INTERRUPT void async_page_fault(int_frame * frame)
{
        EXC_START;
        catch_regs(frame, (void *)get_SP());
        kpanic(exception_messages[PAGE_FAULT]);
};

INTERRUPT void spurious_interrupt_bug(int_frame * frame)
{
        EXC_START;
        catch_regs(frame, (void *)get_SP());
        kpanic(exception_messages[UNDEFINED]);
};

INTERRUPT void coprocessor_error(int_frame * frame)
{
        EXC_START;
        catch_regs(frame, (void *)get_SP());
        kpanic(exception_messages[COPROCESSOR_ERROR]);
};

INTERRUPT void alignment_check(int_frame * frame, uword_t err)
{
        EXC_START;
        kprintf("ERROR CODE: 0x%x", err);
        catch_regs(frame, (void *)get_SP());
        kpanic(exception_messages[ALIGMENT_CHECK]);
};

INTERRUPT void undefined(int_frame * frame)
{
        (void)frame;
        /*
        EXC_START;
        catch_regs(frame, (void *)get_SP());
        kpanic(exception_messages[UNDEFINED]);
        */
};

INTERRUPT void machine_check(int_frame * frame)
{
        EXC_START;
        catch_regs(frame, (void *)get_SP());
        kpanic(exception_messages[MACHINE_CHECK]);
};

INTERRUPT void simd_coprocessor_error(int_frame * frame)
{
        EXC_START;
        catch_regs(frame, (void *)get_SP());
        kpanic(exception_messages[SIMD_CPU_ERROR]);
};
