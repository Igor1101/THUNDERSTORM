#ifndef GCC_OPT_H
#define GCC_OPT_H

#define FORCE_INLINE __attribute__ ( ( always_inline ) ) inline
#define NORET __attribute__ ( ( noreturn ) ) 
#define PACKED __attribute__ ( ( packed ) ) 
#define INTERRUPT __attribute__ ( ( interrupt ) )
#define UNLIKELY __attribute__ ( ( cold ) )
#define LIKELY __attribute__ ( ( hot ) )
#define ALIGN(X) __attribute__ ( ( aligned ( X ) ) )
#define VISIBLE __attribute__ ( ( externally_visible ) )
#define NOOPTIMIZE __attribute__ ( ( optimize("O0") ) )

#endif /* GCC_OPT_H */
