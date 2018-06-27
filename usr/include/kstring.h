#include <ansidecl.h>
#include <stddef.h>
#include <gcc_opt.h>
/* copy (ptr * 16) -aligned data (faster than memcpy ) */
void* kmemcpy_ptr(void *dest, const void *src, size_t len);
/* memset with progress */
PTR kmemset_show (PTR dest, register int val, register size_t len);

