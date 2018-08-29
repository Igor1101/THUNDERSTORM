#include <stdint.h>
#include <compiler_opt.h>
#include <stddef.h>
#include <kstdio.h>
#include <TH/kernel.h>
#include <TH/sysvars.h>
#include <asm/serial.h>

LIKELY int write(int fd, const void *buf, size_t count)
{
        if(fd == WRITE) {
                register unsigned char *ptr = (unsigned char *)buf;
                while (count-- > 0)
                        text.putchar(*ptr++);
                return ((void*)ptr - (void*)buf);
        }
        else if(fd == SERIAL_ONLY) {
                register unsigned char *ptr = (unsigned char *)buf;
                while (count-- > 0)
                        serial_write_asyn(SERIAL_MAIN, *ptr++);
                return ((void*)ptr - (void*)buf);
        }

        return 0;
}
