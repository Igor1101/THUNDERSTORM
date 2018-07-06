/*
 * Copyright (C) 2018  Igor Muravyov <igor.muravyov.2015@gmail.com>
 */
#include <gcc_opt.h>
#include <kstdio.h>
#include <misc/convert.h>
#include <TH/lld.h>
#include <asm/kpanic.h>
#include <asm/cpu_management.h>
#include <x86_64/idt.h>

/*
 * TAB=2 SPACES
 * After exception\ int occured we have in stack:
 * SS                         
 * RSP                        
 * RFLAGS                     
 * CS                         
 * RIP                      
 * ERRCODE(optional)        +80
 * R11  |---> pushed by gcc +72
 * R10  |                   +64
 * R9   |                   +56
 * R8   |                   +48
 * RDI  |                   +40
 * RSI  |                   +32
 * RCX  |                   +24
 * RDX  |                   +16
 * RAX  |                   +8
 * <---------------CURRENT RSP
 */
UNLIKELY NORET void kpanic(char *reason)
{
        select_bgcolor(Red);
        kprintf("\nKERNEL PANIC:");
        select_bgcolor(DefaultBG);
        kprintf("reason: %s\n", reason);
        kprintf("RIP=0x%x,\nCS=0x%x,\nSS=0x%x,\nRFLAGS=0x%x,\nRSP=0x%x\n",
                int_regs.fr.rip,
                int_regs.fr.cs,
                int_regs.fr.ss, int_regs.fr.rflags, int_regs.fr.rsp);
        kprintf("GENERAL PURPOSE REGS: \
\nRAX=0x%x,\
\nRDX=0x%x,\
\nRCX=0x%x,\
\nRSI=0x%x,\
\nRDI=0x%x,\
\nR8=0x%x,\
\nR9=0x%x,\
\nR10=0x%x,\
\nR11=0x%x,\n\
INTERRUPT RSP=0x%x\n", int_regs.rax, int_regs.rdx, int_regs.rcx, int_regs.rsi, int_regs.rdi, int_regs.r8, int_regs.r9, int_regs.r10, int_regs.r11, int_regs.rsp_cur);
        cpu_halt();
}

void catch_regs(int_frame * regs, void *current_SP)
{
        int_regs = (__int_regs) {
        .rax = *(uint64_t *) (current_SP),.rdx =
                    *(uint64_t *) (current_SP + 8),.rcx =
                    *(uint64_t *) (current_SP + 16),.rsi =
                    *(uint64_t *) (current_SP + 24),.rdi =
                    *(uint64_t *) (current_SP + 32),.r8 =
                    *(uint64_t *) (current_SP + 40),.r9 =
                    *(uint64_t *) (current_SP + 48),.r10 =
                    *(uint64_t *) (current_SP + 56),.r11 =
                    *(uint64_t *) (current_SP + 64),.rsp_cur =
                    (uint64_t) current_SP};
        int_regs.fr = *regs;
}
