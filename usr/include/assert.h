#ifndef ASSERT_H
#define ASSERT_H

#ifdef __cplusplus
extern "C" {
#endif

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
#endif /* DEBUG */

#ifdef __cplusplus
}
#endif

#endif /* ASSERT_H */
