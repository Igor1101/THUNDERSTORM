/* memset
   This implementation is in the public domain.  */

/*
@deftypefn Supplemental void* memset (void *@var{s}, int @var{c}, @
  size_t @var{count})
Sets the first @var{count} bytes of @var{s} to the constant byte
@var{c}, returning a pointer to @var{s}.
@end deftypefn
*/

#include <kstdlib.h>
#include <gcc_opt.h>
#include <misc/show.h>

LIKELY void *memset(void *dest, register int val, register size_t len)
{
	register unsigned char *ptr = (unsigned char *)dest;
	while (len-- > 0)
		*ptr++ = val;
	return dest;
}

/* the same as memset, also showing progress */
LIKELY void *kmemset_show(void *dest, register int val, register size_t len)
{
	init_progress();
	register unsigned char *ptr = (unsigned char *)dest;
	while (len-- > 0) {
		if (len % 100 == 0)
			show_progress();
		*ptr++ = val;
	}
	stop_progress();
	return dest;
}
