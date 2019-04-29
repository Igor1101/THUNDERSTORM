/*
 * Copyright (C) 2018-2019  Igor Muravyov <igor.muravyov.2015@gmail.com>
 */
#ifndef DIE_H
#define DIE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <compiler_opt.h>
NORET void die(char *reason);   /*  assembly function */
NORET void die_stdio(char *reason);
/* write message, using kprintf*/

#ifdef __cplusplus
}
#endif

#endif                          /* DIE_H */
