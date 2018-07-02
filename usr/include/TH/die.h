#ifndef DIE_H
#define DIE_H

#include <gcc_opt.h>
NORET void die(char*reason);/*  assembly function */
NORET void die_stdio(char*reason);
/* write message, using kprintf*/
#endif /* DIE_H */
