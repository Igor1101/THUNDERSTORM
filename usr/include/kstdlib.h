#ifndef KSTDLIB
#define KSTDLIB


#ifdef __cplusplus
extern "C" {
#endif

<<<<<<< HEAD
#include <stdint.h>
#include <stddef.h>
=======
#include "sys/types.h"
#include <stdint.h>
>>>>>>> fbc8d093eb777ef79fbe6b6d86d5d2412ae235f2

#define ITOA_BUF_SIZE 33

void itoa(char *buf, intptr_t base, intptr_t value);
void itoap(char *buf, size_t width, intptr_t base, uintptr_t value);
void utoa(char *buf, uintptr_t value);

/* misc but declared here: */
void show_progress(void);
void init_progress(void);
void stop_progress(void);


#ifdef __cplusplus
}
#endif

#endif /* KSTDLIB */
