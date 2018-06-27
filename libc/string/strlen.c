<<<<<<< HEAD
#include <stddef.h>
=======
#include <stddef.h> /* for size_t */
>>>>>>> fbc8d093eb777ef79fbe6b6d86d5d2412ae235f2
#include <gcc_opt.h>
LIKELY size_t strlen(const char *s) {
    size_t i;
    for (i = 0; s[i] != '\0'; i++) ;
    return i;
}
