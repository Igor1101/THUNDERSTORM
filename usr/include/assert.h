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

FORCE_INLINE void aFailed_release(char* file, int line)
{
        pr_err("\
                        \nWe are so sorry, but it looks like \nthis release is not stable(assertion failed) : \
                        \n file: %s, line: %d \
                        \nif you are just user and see this error, \
                        \nplease contact <igor.muravyov.2015@gmail.com> ",
                        file, line);
}
#ifdef DEBUG
#define ASSERT(expr) \
        if (!(expr)) \
        aFailed(__FILE__, __LINE__)
#elif defined RELEASE
        #define ASSERT(expr) \
        if (!(expr)) \
        aFailed_release(__FILE__, __LINE__)
#endif /* DEBUG */

#ifdef __cplusplus
}
#endif

#endif /* ASSERT_H */
