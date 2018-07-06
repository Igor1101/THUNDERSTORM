/*
 * Copyright (C) 2018  Igor Muravyov <igor.muravyov.2015@gmail.com>
 */
#include <stddef.h>
#include <stdint.h>
#include <gcc_opt.h>

/*
 * This is too slow for video memory 
 */
LIKELY void *memcpy(void *dest, const void *src, size_t len)
{
	register volatile char *d = dest;
	register volatile const char *s = src;
	while (len--)
		*d++ = *s++;
	return dest;
}

/*
 * TH kernel memcpy, note, that this function can 
 * copy correctly only uintptr_t * 16 aligned data;
 * used for copying videomemory areas
 */
LIKELY void *kmemcpy_ptr(void *dest, const void *src, size_t len)
{
	register volatile uint32_t length = len / sizeof(uintptr_t) / 16;	/* size in ptrs * 16 */
	register volatile uintptr_t *d = dest;
	register volatile const uintptr_t *s = src;
	while (length -= 1) {

		*d++ = *s++;	//1
		*d++ = *s++;	//2
		*d++ = *s++;	//3
		*d++ = *s++;	//4
		*d++ = *s++;	//5
		*d++ = *s++;	//6
		*d++ = *s++;	//7
		*d++ = *s++;	//8
		*d++ = *s++;	//9
		*d++ = *s++;	//10
		*d++ = *s++;	//11
		*d++ = *s++;	//12
		*d++ = *s++;	//13
		*d++ = *s++;	//14
		*d++ = *s++;	//15
		*d++ = *s++;	//16

	}
	return dest;
}
