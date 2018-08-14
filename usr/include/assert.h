#ifndef ASSERT_H
#define ASSERT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <compiler_opt.h>
#include <kstdio.h>
#include <TH/printk.h>

FORCE_INLINE void aFailed(char* file, int line)
{
        pr_err("assertion failed: \n file: %s, line: %d", file, line);
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
