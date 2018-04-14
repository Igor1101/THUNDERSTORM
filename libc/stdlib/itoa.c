#include <stdint.h>
#include <stddef.h>
#include <kstdlib.h>
#include <string.h>

static const char *s_itoa_chars = "0123456789abcdef";

void itoa(char *buf, int base, int val) {
    if (!val) {
        buf[0] = '0';
        buf[1] = 0;
        return;
    }
    int sign = 0;
    uint32_t value;

    if (base == 10 && val < 0) {
        sign = 1;
        value = -val;
    } else {
        value = val;
    }

    int r, c, j;
    c = 0;
    while (value) {
        buf[c++] = s_itoa_chars[value % base];
        value /= base;
    }
    if (sign) {
        r = c + 1;
        buf[c] = '-';
    } else {
        r = c;
        --c;
    }

    j = 0;
    while (c > j) {
        buf[c] ^= buf[j];
        buf[j] ^= buf[c];
        buf[c] ^= buf[j];
        --c;
        ++j;
    }

    buf[r] = 0;
}

void itoap(char *buf, size_t width, int base, uint32_t value) {
    memset(buf, '0', width);
    buf[width] = 0;
    int i = 0;
    while (value) {
        buf[width - i - 1] = s_itoa_chars[value % base];
        value /= base;
        ++i;
    }
}

void utoa(char *buf, uint32_t value) {
    if (!value) {
        buf[0] = '0';
        buf[1] = 0;
        return;
    }

    int r, c, j;
    c = 0;
    while (value) {
        buf[c++] = s_itoa_chars[value % 10];
        value /= 10;
    }

    r = c;
    --c;

    j = 0;
    while (c > j) {
        buf[c] ^= buf[j];
        buf[j] ^= buf[c];
        buf[c] ^= buf[j];
        --c;
        ++j;
    }

    buf[r] = 0;
}
