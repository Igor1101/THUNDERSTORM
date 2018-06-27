/* memset
   This implementation is in the public domain.  */

/*
@deftypefn Supplemental void* memset (void *@var{s}, int @var{c}, @
  size_t @var{count})
Sets the first @var{count} bytes of @var{s} to the constant byte
@var{c}, returning a pointer to @var{s}.
@end deftypefn
*/

<<<<<<< HEAD
#include <kstdlib.h>
#include <gcc_opt.h>

LIKELY void*
memset (void* dest, register int val, register size_t len)
=======
#include <ansidecl.h>
#include <stddef.h>
#include <kstdlib.h>
#include <gcc_opt.h>

LIKELY PTR 
memset (PTR dest, register int val, register size_t len)
>>>>>>> fbc8d093eb777ef79fbe6b6d86d5d2412ae235f2
{
  register unsigned char *ptr = (unsigned char*)dest;
  while (len-- > 0)
    *ptr++ = val;
  return dest;
}

/* the same as memset, also showing progress */
<<<<<<< HEAD
LIKELY void*
kmemset_show (void* dest, register int val, register size_t len)
=======
LIKELY PTR 
kmemset_show (PTR dest, register int val, register size_t len)
>>>>>>> fbc8d093eb777ef79fbe6b6d86d5d2412ae235f2
{
  init_progress();
  register unsigned char *ptr = (unsigned char*)dest;
  while (len-- > 0)
  {
<<<<<<< HEAD
    if( len % 100 == 0)
=======
    if( len % 10 == 0)
>>>>>>> fbc8d093eb777ef79fbe6b6d86d5d2412ae235f2
      show_progress();
    *ptr++ = val;
  }
  stop_progress();
  return dest;
}
