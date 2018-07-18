#ifndef ASSERT_H
#define ASSERT_H

#include <gcc_opt.h>
#include <kstdio.h>

FORCE_INLINE void aFailed(char* file, int line)
{
        kprintf("assertion failed: \n file: %s, line: %d", file, line);
}
#ifdef DEBUG
#define ASSERT(expr) \
        if (!(expr)) \
        aFailed(__FILE__, __LINE__)
#elif
        #define ASSERT
#endif
#endif /* ASSERT_H */
