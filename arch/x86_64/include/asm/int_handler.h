/*
 * Copyright (C) 2018  Igor Muravyov <igor.muravyov.2015@gmail.com>
 */
#ifndef INT_HANDLER_H
#define INT_HANDLER_H

#include <kstdio.h>
#include <TH/lld.h>
#include <TH/kernel.h>
#include <asm/cpu_management.h>
#define EXC_START \
  pr_warn("TH EXCEPTION:");
                                //clear_interrupts();
#define EXC_EXIT \
                                //set_interrupts();

void set_exceptions(void);
void early_init_interrupts(void);
void init_interrupts(void);

#endif                          /* INT_HANDLER_H */
