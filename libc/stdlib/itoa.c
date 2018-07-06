/* 
 * this file has been copied 
 * by Igor Muravyov <igor.muravyov.2015@gmail.com>
 * from x86k <https://github.com/alnyan/x86k> 
 * project (made by alnyan <alnyan@protonmail.com>)
 *  * to THUNDERSTORM sources(NO LICENSE)
 */
#include <stdint.h>
#include <kstdlib.h>
#include <kstring.h>
#include <stddef.h>
#include <gcc_opt.h>
static const char *s_itoa_chars = "0123456789abcdef";

LIKELY void itoa(char *buf, intptr_t base, intptr_t val)
{
	if (!val) {
		buf[0] = '0';
		buf[1] = 0;
		return;
	}
	intptr_t sign = 0;
	uintptr_t value;

	if (base == 10 && val < 0) {
		sign = 1;
		value = -val;
	} else {
		value = val;
	}

	intptr_t r, c, j;
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

LIKELY void itoap(char *buf, size_t width, intptr_t base, uintptr_t value)
{
	memset(buf, '0', width);
	buf[width] = 0;
	intptr_t i = 0;
	while (value) {
		buf[width - i - 1] = s_itoa_chars[value % base];
		value /= base;
		++i;
	}
}

LIKELY void utoa(char *buf, uintptr_t value)
{
	if (!value) {
		buf[0] = '0';
		buf[1] = 0;
		return;
	}

	intptr_t r, c, j;
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
