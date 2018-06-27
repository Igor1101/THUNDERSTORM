<<<<<<< HEAD
=======
#include <ansidecl.h>
>>>>>>> fbc8d093eb777ef79fbe6b6d86d5d2412ae235f2
#include <stddef.h>
#include <gcc_opt.h>
/* copy (ptr * 16) -aligned data (faster than memcpy ) */
void* kmemcpy_ptr(void *dest, const void *src, size_t len);
/* memset with progress */
<<<<<<< HEAD
void *kmemset_show (void* dest, register int val, register size_t len);
void *memset (void* dest, register int val, register size_t len);

void * memcpy (void *dest, const void *src, size_t len);
size_t strlen(const char *s);
=======
PTR kmemset_show (PTR dest, register int val, register size_t len);

>>>>>>> fbc8d093eb777ef79fbe6b6d86d5d2412ae235f2
