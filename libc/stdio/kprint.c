/* *
 * copied from x86k system <https://github.com/alnyan/x86k> 
 * Authors: Alnyan <alnyan@protonmail.com>
 * Modified: Igor1101
 * (PUBLIC DOMAIN)
 * modified for THUNDERSTORM System
 * */
#include <stdint.h>
#include <kstring.h>
#include <stdarg.h>
#include <kstdlib.h>
#include <kstdio.h>
#include <TH/lld.h>
#include <TH/sysvars.h>
#include <compiler_opt.h>

LIKELY void kputs(const char *s)
{
        while (*s) {
                text.putchar(*s++);
        }
}

LIKELY void kputs_pad(const char *s, char c, size_t p)
{
        size_t d = strlen(s);
        if (d < p) {
                for (size_t i = 0; i < p - d; ++i) {
                        text.putchar(c);
                }
        }
        kputs(s);
}

LIKELY void kdump(const void *block, size_t count)
{
        const char *v = (const char *)block;
        unsigned char c;
        static char cbuf[16];
        for (size_t off = 0; off < count; off += 16) {
                itoap(cbuf, 8, 16, off);
                kputs(cbuf);
                kputs(": ");
                for (size_t i = 0; i < 16 && off + i < count; ++i) {
                        c = v[i + off];
                        itoap(cbuf, 2, 16, c);
                        kputs(cbuf);
                        text.putchar(' ');
                }

                kputs("   ");

                for (size_t i = 0; i < 16 && off + i < count; ++i) {
                        c = v[i + off];
                        if (!c) {
                                text.putchar('.');
                        } else if (c > 32 && c < 0x7F) {
                                text.putchar(c);
                        } else {
                                text.putchar('?');
                        }
                }
                text.putchar('\n');
        }
}

void kprintf(const char *fmt, ...)
{
        va_list args;
        va_start(args, fmt);
        kvprintf(fmt, args);
        va_end(args);
}

LIKELY WEAK int vsnprintf (char * s, size_t n, 
                const char * fmt, va_list args )
{
        /* it does not seem to get overflowed */
#define CBUF_SIZE 256
        char c;
        uintptr_t vp;
        static char cbuf[CBUF_SIZE];
        char padc;
        int padn;
        char*buf_p = s;

        auto int putchar_buf(char c);
        auto int puts_pad_buf(const char *s, char c, size_t p);
        auto int putchar_buf(char c);
        auto int puts_buf(const char *s);

        auto int putchar_buf(char c)
        {
                *(buf_p++) = c;
                if( (uintptr_t)(buf_p - s) >= n )
                        return EXIT_FAILURE;
                return EXIT_SUCCESS;
        }

        auto int puts_pad_buf(const char *s, char c, size_t p)
        {
                size_t d = strlen(s);
                if (d < p) {
                        for (size_t i = 0; i < p - d; ++i) {
                                if(putchar_buf(c) == EXIT_FAILURE) {
                                        return EXIT_FAILURE;
                                }
                        }
                }
                if(puts_buf(s) == EXIT_FAILURE) {
                        return EXIT_FAILURE;
                }
                return EXIT_SUCCESS;
        }

        auto int puts_buf(const char *s)
        {
                while (*s) {
                        if(putchar_buf(*s++) == EXIT_FAILURE) {
                                return EXIT_FAILURE;
                        }
                }
                return EXIT_SUCCESS;
        }

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
                                        if(puts_pad_buf(cbuf, padc, padn) 
                                                        == EXIT_FAILURE) {
                                                return (buf_p - s);
                                        }
                                        break;
                                default:
                                        if(putchar_buf('%') == EXIT_FAILURE ) {
                                                return (buf_p - s);
                                        }
                                        if(putchar_buf(c) == EXIT_FAILURE ) {
                                                return (buf_p - s);
                                        }
                                        break;
                                }
                        } else {
                                switch (c) {
                                case 'a':
                                        vp = va_arg(args, uintptr_t);
                                        itoa(cbuf, 16, vp);
                                        if(puts_buf("0x") == EXIT_FAILURE)
                                                return (buf_p - s);
                                        if(puts_pad_buf(cbuf, '0', 8) == EXIT_FAILURE)
                                                return (buf_p - s);
                                        break;
                                case 'd':
                                        vp = va_arg(args, uintptr_t);
                                        itoa(cbuf, 10, vp);
                                        if(puts_buf(cbuf) == EXIT_FAILURE) 
                                                return (buf_p - s);
                                        break;
                                case 'x':
                                        vp = va_arg(args, uintptr_t);
                                        itoa(cbuf, 16, vp);
                                        if(puts_buf(cbuf) == EXIT_FAILURE)
                                                return (buf_p - s);
                                        break;
                                case 'u':
                                        vp = va_arg(args, uintptr_t);
                                        utoa(cbuf, vp);
                                        if(puts_buf(cbuf) == EXIT_FAILURE)
                                                return (buf_p - s);
                                        break;
                                case 's':
                                        vp = (uintptr_t) va_arg(args,
                                                                const char *);
                                        if(puts_buf(vp ? (const char *)vp : "(NULL)")
                                                        == EXIT_FAILURE)
                                                return (buf_p - s);
                                        break;
                                default:
                                        if(putchar_buf('%') == EXIT_FAILURE)
                                                return (buf_p - s);
                                        if(putchar_buf(c) == EXIT_FAILURE)
                                                return (buf_p - s);
                                        break;
                                }
                        }
                        break;
                default:
                        if(putchar_buf(c) == EXIT_FAILURE) 
                                return (buf_p - s);
                        break;
                }
                ++fmt;
        }
        return (buf_p - s);
}

LIKELY void kvprintf(const char *fmt, va_list args)
{
        /* it does not seem to be get overflowed */
#define CBUF_SIZE 256
        char c;
        uintptr_t vp;
        char cbuf[CBUF_SIZE];
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
                                        kputs_pad(cbuf, padc, padn);
                                        break;
                                default:
                                        text.putchar('%');
                                        text.putchar(c);
                                        break;
                                }
                        } else {
                                switch (c) {
                                case 'a':
                                        vp = va_arg(args, uintptr_t);
                                        itoa(cbuf, 16, vp);
                                        kputs("0x");
                                        kputs_pad(cbuf, '0', 8);
                                        break;
                                case 'd':
                                        vp = va_arg(args, uintptr_t);
                                        itoa(cbuf, 10, vp);
                                        kputs(cbuf);
                                        break;
                                case 'x':
                                        vp = va_arg(args, uintptr_t);
                                        itoa(cbuf, 16, vp);
                                        kputs(cbuf);
                                        break;
                                case 'u':
                                        vp = va_arg(args, uintptr_t);
                                        utoa(cbuf, vp);
                                        kputs(cbuf);
                                        break;
                                case 's':
                                        vp = (uintptr_t) va_arg(args,
                                                                const char *);
                                        kputs(vp ? (const char *)vp : "(null)");
                                        break;
                                default:
                                        text.putchar('%');
                                        text.putchar(c);
                                        break;
                                }
                        }
                        break;
                default:
                        text.putchar(c);
                        break;
                }
                ++fmt;
        }
}
