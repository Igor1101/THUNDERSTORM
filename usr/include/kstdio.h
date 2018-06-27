#ifndef KSTDIOB
#define KSTDIOB

#ifdef __cplusplus
extern "C" {
#endif


#include <stdint.h>

#include <stdarg.h>
void kprintf(const char *fmt, ...);
void kvprintf(const char *fmt, va_list args);
void kputchar(int8_t chr);
void kputs(char* str);


#ifdef __cplusplus
}
#endif

#endif /* KSTDIOB */
