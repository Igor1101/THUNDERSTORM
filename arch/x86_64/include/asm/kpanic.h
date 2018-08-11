/*
 * Copyright (C) 2018  Igor Muravyov <igor.muravyov.2015@gmail.com>
 */
#ifndef KPANIC_H
#define KPANIC_H

#include <gcc_opt.h>
#include <kstdio.h>
#include <x86_64/IDT.h>

extern struct __regs_int_gcc {
        int_frame fr;
        uint64_t rax;
        uint64_t rdx;
        uint64_t rcx;
        uint64_t rsi;
        uint64_t rdi;
        uint64_t r8;
        uint64_t r9;
        uint64_t r10;
        uint64_t r11;
        /*interrupt current stack pointer */
        uint64_t rsp_cur;
} int_regs;

typedef struct __regs_int_gcc __int_regs;

void kpanic(char *reason);

void catch_regs(int_frame * regs, void *current_SP);

#endif                          /* KPANIC_H */
