#include <stddef.h>
#include <gcc_opt.h>
LIKELY size_t strlen(const char *s)
{
	size_t i;
	for (i = 0; s[i] != '\0'; i++) ;
	return i;
}
