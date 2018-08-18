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


#ifdef _NC_RESTRICT
char *strcpy(char *restrict dest, const char *restrict src)
#else
char *strcpy(char *dest, const char* src)
#endif
{
    char *ret = dest;
    while ( (*(dest++) = *(src++) ) )
        ;
    return ret;
}
