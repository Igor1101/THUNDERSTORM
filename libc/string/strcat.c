#include <stddef.h> /* size_t */

char *strcat(char *dest, const char *src)
{
    char *ret = dest;
    while (*dest)
        dest++;
    while ( (*(dest++) = *(src++) )  )
        ;
    return ret;
}

char *strncat(char *dest, const char *src, size_t n)
{
    char *ret = dest;
    while (*dest)
        dest++;
    while (n--)
        if (!(*(dest++) = *(src++) ) )
            return ret;
    *dest = 0;
    return ret;
}
