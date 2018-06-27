#ifndef KSTDLIB
#define KSTDLIB


#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>

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
