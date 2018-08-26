/*
 * Copyright (C) 2018  Igor Muravyov <igor.muravyov.2015@gmail.com>
 */
#ifndef GCC_OPT_H
#define GCC_OPT_H

#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif

#define FORCE_INLINE __attribute__ ( ( always_inline ) ) inline
#define NORET __attribute__ ( ( noreturn ) )
#define PACKED __attribute__ ( ( packed ) )
#define INTERRUPT __attribute__ ( ( interrupt ) )
#define UNLIKELY __attribute__ ( ( cold ) )
#define LIKELY __attribute__ ( ( hot ) )
#define ALIGNED(X) __attribute__ ( ( aligned ( X ) ) )
#define VISIBLE __attribute__ ( ( externally_visible ) )
#define NOOPTIMIZE __attribute__ ( ( optimize("O0") ) )
#define __init __attribute__ ( ( section(".init_text") ) )
#define HIDDEN __attribute__ ( ( visibility("hidden") ) )
#define WEAK __attribute__((weak))
/* For suitability with linux attributes */
#define __cold UNLIKELY
#define __hot LIKELY
#define __visible VISIBLE
#define __user         __attribute__((noderef, address_space(1)))
#define __aligned(X)  ALIGNED(X)
#define __weak WEAK
        typedef uint64_t u64;
        typedef int64_t i64;
        typedef uint32_t u32;
        typedef int32_t i32;
        typedef uint16_t u16;
        typedef int16_t i16;
        typedef uint8_t u8;
        typedef int8_t i8;
        typedef u64 loff_t;

#ifdef __linux__
         #undef __linux__
#endif /* linux */
#ifndef __THUNDERSTORM__
#define __THUNDERSTORM__
#endif /*THUNDERSTORM */

#ifdef __cplusplus
}
#endif

#endif                          /* GCC_OPT_H */
