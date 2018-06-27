#include <stddef.h>
#include <gcc_opt.h>
/* copy (ptr * 16) -aligned data (faster than memcpy ) */
void* kmemcpy_ptr(void *dest, const void *src, size_t len);
/* memset with progress */
void *kmemset_show (void* dest, register int val, register size_t len);
void *memset (void* dest, register int val, register size_t len);

void * memcpy (void *dest, const void *src, size_t len);
size_t strlen(const char *s);
