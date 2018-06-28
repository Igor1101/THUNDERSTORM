#ifndef GCC_OPT
#define GCC_OPT

#define FORCE_INLINE __attribute__ ( ( always_inline ) ) inline
#define NORET __attribute__ ( ( noreturn ) ) 
#define PACKED __attribute__ ( ( packed ) ) 
#define INTERRUPT __attribute__ ( ( interrupt ) ) 
#define UNLIKELY __attribute__ ( ( cold ) )
#define LIKELY __attribute__ ( ( hot ) )
#define ALIGN(X) __attribute__ ( ( aligned ( X ) ) )

#endif
