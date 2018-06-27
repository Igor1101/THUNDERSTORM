#ifndef KSTDIOB
#define KSTDIOB

#ifdef __cplusplus
extern "C" {
#endif

<<<<<<< HEAD

#include <stdint.h>

=======
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
>>>>>>> fbc8d093eb777ef79fbe6b6d86d5d2412ae235f2
#include <stdarg.h>
void kprintf(const char *fmt, ...);
void kvprintf(const char *fmt, va_list args);
void kputchar(int8_t chr);
void kputs(char* str);


#ifdef __cplusplus
}
#endif

#endif /* KSTDIOB */
