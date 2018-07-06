/*
 * Copyright (C) 2018  Igor Muravyov <igor.muravyov.2015@gmail.com>
 */
#ifndef LINKER_INFO_H
#define LINKER_INFO_H

extern char kernel_phys_base;
extern char kernel_virt_base;
extern char kernel_text;
extern char kernel_data;
extern char init_kernel_bss;
extern char init_kernel_bss_end;
extern char kernel_bss;
extern char kernel_bss_end;

#endif                          /* LINKER_INFO_H */
