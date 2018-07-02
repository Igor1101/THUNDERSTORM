/*
 * Copyright (C) 2018  Igor Muravyov <igor.muravyov.2015@gmail.com>
 */
#ifndef DEV_INIT_H
#define DEV_INIT_H

/* this header contains run-time
 * info about what bootinfo() could
 * initialize via bootloader provided information
 * and coudn`t
 */
extern bool raminfo_initialized;//\___> system crash if false
extern bool memmap_initialized; ///
extern bool videomode_initialized;
extern bool cmdline_initialized;

#endif /* DEV_INIT_H */
