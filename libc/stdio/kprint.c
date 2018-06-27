/* copied from x86k system */
#include <stdint.h>
<<<<<<< HEAD
#include <kstring.h>
=======
#include <string.h>
>>>>>>> fbc8d093eb777ef79fbe6b6d86d5d2412ae235f2
#include <stdarg.h>
#include <kstdlib.h>
#include <kstdio.h>
#include <TH/lld.h>
#include <gcc_opt.h>
#define kputc kputchar

LIKELY void dputs(const char *s) {
#ifdef KDEBUG
  kpause();
#endif
    while (*s) {
        kputc(*s++);
    }
}

LIKELY void dputs_pad(const char *s, char c, size_t p) {
    size_t d = strlen(s);
    if (d < p) {
        for (size_t i = 0; i < p - d; ++i) {
            kputc(c);
        }
    }
    dputs(s);
}

LIKELY void ddump(const void *block, size_t count) {
    const char *v = (const char *) block;
    unsigned char c;
    static char cbuf[16];
    for (size_t off = 0; off < count; off += 16) {
        itoap(cbuf, 8, 16, off);
        dputs(cbuf);
        dputs(": ");
        for (size_t i = 0; i < 16 && off + i < count; ++i) {
            c = v[i + off];
            itoap(cbuf, 2, 16, c);
            dputs(cbuf);
            kputc(' ');
        }

        dputs("   ");

        for (size_t i = 0; i < 16 && off + i < count; ++i) {
            c = v[i + off];
            if (!c) {
                kputc('.');
            } else if (c > 32 && c < 0x7F) {
                kputc(c);
            } else {
                kputc('?');
            }
        }
        kputc('\n');
    }
}

void kprintf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    kvprintf(fmt, args);
    va_end(args);
}

LIKELY void kvprintf(const char *fmt, va_list args) {
    char c;
    uintptr_t vp;
    char cbuf[256];
    char padc;
    int padn;

    while ((c = *fmt)) {
        switch (c) {
        case '%':
            ++fmt;
            c = *fmt;
            padc = 0;
            if (c == '0' || c == ' ') {
                padc = c;
                ++fmt;
            }
            padn = 0;
            c = *fmt;
            while (c >= '0' && c <= '9') {
                padn *= 10;
                padn += c - '0';
                c = *++fmt;
            }

            if (padn && padc) {
                switch (c) {
                case 'x':
                    vp = va_arg(args, uintptr_t);
                    itoa(cbuf, 16, vp);
                    dputs_pad(cbuf, padc, padn);
                    break;
                default:
                    kputc('%');
                    kputc(c);
                    break;
                }
            } else {
                switch (c) {
                case 'a':
                    vp = va_arg(args, uintptr_t);
                    itoa(cbuf, 16, vp);
                    dputs("0x");
                    dputs_pad(cbuf, '0', 8);
                    break;
                case 'd':
                    vp = va_arg(args, uintptr_t);
                    itoa(cbuf, 10, vp);
                    dputs(cbuf);
                    break;
                case 'x':
                    vp = va_arg(args, uintptr_t);
                    itoa(cbuf, 16, vp);
                    dputs(cbuf);
                    break;
                case 'u':
                    vp = va_arg(args, uintptr_t);
                    utoa(cbuf, vp);
                    dputs(cbuf);
                    break;
                case 's':
                    vp = (uintptr_t) va_arg(args, const char *);
                    dputs(vp ? (const char *) vp : "(null)");
                    break;
                default:
                    kputc('%');
                    kputc(c);
                    break;
                }
            }
            break;
        default:
            kputc(c);
            break;
        }
        ++fmt;
    }
}

NORET void kpanic(const char *msg) {
    kprintf("!!! PANIC !!!\n%s\n", msg);
    while (1) {
        __asm__ __volatile__ ("cli; hlt");
    }
}
