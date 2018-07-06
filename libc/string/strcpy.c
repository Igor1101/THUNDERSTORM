#include <stddef.h>
char *strncpy(char *dest, const char *src, size_t n)
{
        char *ret = dest;
        do {
                if (!n--)
                        return ret;
        }
        while (((*(dest++)) = *(src++)) != 0);
        while (n--)
                *(dest++) = 0;
        return ret;
}
