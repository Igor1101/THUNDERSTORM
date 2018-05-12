#ifndef KSTDLIB
#define KSTDLIB


#ifdef __cplusplus
extern "C" {
#endif

#include "sys/types.h"
#include <stdint.h>

#define ITOA_BUF_SIZE 33

void itoa(char *buf, intptr_t base, intptr_t value);
void itoap(char *buf, size_t width, intptr_t base, uintptr_t value);
void utoa(char *buf, uintptr_t value);


#ifdef __cplusplus
}
#endif

#endif /* KSTDLIB */
