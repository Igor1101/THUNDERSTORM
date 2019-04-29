/*
 * Copyright (C) 2018-2019  Igor Muravyov <igor.muravyov.2015@gmail.com>
 */
#include <kstdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <TH/lld.h>
#include <TH/sysvars.h>
#include <TH/font.h>
#include <TH/kernel.h>

/* 
 * import psf(PC screen font) , 
 * only psf2 fonts are currently supported (not psfu )
 * this file path (kernel/misc/font.c) should not be changed
 * it uses relative path
 * */

asm(".section .rodata\n"
    ".global _font_start\n"
    "_font_start: \n"
    " .incbin \"../../usr/share/consolefonts/uni.psf\"\n"
    "_font_end: \n" ".section .text\n");

uint16_t *unicode = NULL;
PSF_font *font;

int font_info(void)
{
        volatile int ret = 0;
        printk("fonts info:");
        font = (PSF_font *) & _font_start;
        if (PSF_FONT_MAGIC == font->magic) {
                printk("font is PSF");
        } else {
                printk("Warning: font is not supported by kernel");
                ret = -1;
        }
        printk("version %d\n", font->version);
        printk("headersize %d\n", font->headersize);
        printk("flags %d\n", font->flags);
        printk("numglyph %d\n", font->numglyph);
        printk("height %d\n", font->height);
        printk("width %d\n", font->width);
        return ret;
}
