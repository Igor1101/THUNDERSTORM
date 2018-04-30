#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdarg.h>
void kprintf(const char *fmt, ...);
void kvprintf(const char *fmt, va_list args);
void kputchar(int8_t chr);
void kputs(char* str);
