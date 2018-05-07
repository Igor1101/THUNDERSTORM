#include <stddef.h>

void * 
memcpy (void *dest, const void *src, size_t len)
{
  volatile char *d = dest;
  volatile const char *s = src;
  while (len--)
    *d++ = *s++;
  return dest;
}
