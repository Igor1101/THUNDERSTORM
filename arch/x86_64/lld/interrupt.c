/* 
 * Copyright (C) 2018  Igor Muravyov <igor.muravyov.2015@gmail.com>
 */
#include <assert.h>
#include <asm/traps.h>
#include <asm/kpanic.h>
#include <x86_64/IDT.h>
#include <x86_64/TSS.h>
#include <x86_64/cpu_management.h>
#include <x86_64/APIC.h>
#include <x86_64/PIC.h>
#include <asm/kpanic.h>
#include <asm/int_handler.h>

/* *INDENT-OFF* */
void (*exceptions_array[]) = {
        divide_error,           //0
            debug,              //1
            nmi,                //2
            int3,               /* breakpoit *///3
            overflow,           //4
            bounds,             //5
            invalid_op,         //6
            device_not_available,       //7
            double_fault,       //8
            coprocessor_segment_overrun,        //9 useless
            invalid_TSS,        //10
            segment_not_present,        //11
            stack_segment,      //12
            general_protection, //13
            page_fault,         //14
            undefined, 
            coprocessor_error,       //16  FPU legacy error
            alignment_check,    //17
            machine_check,      //18
            simd_coprocessor_error,     //19
            undefined, 
            undefined, 
            undefined, 
            undefined, 
            undefined, 
            undefined, 
            undefined, 
            undefined, 
            undefined, 
            undefined, 
            undefined, 
            undefined  //31
};
/* *INDENT-ON* */
UNLIKELY void verify_ints(void);

UNLIKELY void early_init_interrupts(void)
{
        set_exceptions();
        lidt(idt_table, EARLY_SIZE_OF_IDT);
        verify_ints();
        init_tss();
        idt_clear_vectors();
        set_exceptions();
}

/**
 * int verify_ints(void)
 * @return: 0 if interrupts are working
 * -1 otherwise
 *  */
UNLIKELY void verify_ints(void)
{
        idt_set_trap(0, (uint64_t)verify_handler, 0);
        asm(
                        "movq $0x0123456789, %rax\n"
                        "movq $0x0123456789, %rdx\n"
                        "int $0x0"
                        );
        ASSERT(int_regs.rax == 0x0123456789);
        ASSERT(int_regs.rdx == 0x0123456789);
}

UNLIKELY void set_exceptions(void)
{
        for (uint32_t num = 0; num < NUM_OF_EXCEPTIONS; num++) {
                idt_set_trap(num, (uint64_t) exceptions_array[num], 1);
        }
        idt_set_trap(DOUBLE_FAULT, (uint64_t) exceptions_array[DOUBLE_FAULT], 7);
        idt_set_trap(GENERAL_PROTECTION, 
                        (uint64_t) exceptions_array[GENERAL_PROTECTION], 6);
        idt_set_trap(NMI, (uint64_t) exceptions_array[NMI], 5);
        idt_set_trap(PAGE_FAULT, (uint64_t) exceptions_array[PAGE_FAULT], 4);
        /* set TSS descriptor in GDT table*/
        set_tss_desc();
        set_tss_table();
}

UNLIKELY void init_interrupts(void)
{        
        pic_remap(IRQ0_OFFSET, IRQ1_OFFSET);
        /* APIC \ PIC initializations */
        if(apic_present()) {
                pr_notice("APIC is present");
#ifdef USE_APIC
                pic_disable();
#endif
        } else {
                pr_err("APIC is not present");
#ifdef USE_APIC
                kpanic("APIC is not present");
#endif
        }
        apic_get_info_ACPI();
        boot_lapic_enable();
}
