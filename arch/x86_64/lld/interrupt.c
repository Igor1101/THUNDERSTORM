/* 
 * Copyright (C) 2018  Igor Muravyov <igor.muravyov.2015@gmail.com>
 */
#include <asm/traps.h>
#include <x86_64/idt.h>
#include <x86_64/tss.h>
#include <x86_64/cpu_management.h>

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

UNLIKELY void init_interrupts(void)
{
        lidt(idt_table, EARLY_SIZE_OF_IDT);
        init_tss();
}

UNLIKELY void set_exceptions(void)
{
        idt_clear_vectors();
        
        for (uint32_t num = 0; num < NUM_OF_EXCEPTIONS; num++) {
                idt_set_trap(num, (uint64_t) exceptions_array[num], 0);
        }
        //idt_set_trap(6, (uint64_t)exceptions_array[6],1);
        idt_set_trap(INVALID_OP, (uint64_t)exceptions_array[INVALID_OP],1);
        /* set TSS descriptor in GDT table*/
        set_tss_desc();
        set_tss_table();
}
