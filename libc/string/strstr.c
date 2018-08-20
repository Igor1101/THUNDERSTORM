#include <kstring.h> /* size_t memcmp() strlen() */
char *strstr(char *s1, char *s2)
{
    size_t n = strlen(s2);
    while(*s1)
        if(!memcmp(s1++,s2,n))
            return s1-1;
    return 0;
}
