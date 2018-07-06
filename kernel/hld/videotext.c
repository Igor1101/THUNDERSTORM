/*
 * Copyright (C) 2018  Igor Muravyov <igor.muravyov.2015@gmail.com>
 */
#include <kstdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <TH/lld.h>
#include <TH/sysvars.h>
#include <TH/font.h>
#include <video_lld.h>

#ifdef USE_VESA

int verify_addr(uint32_t * addr)
  /* verify if we are writing to permissible address */
{
        if (addr >= (uint32_t *) sysfb.virtaddr + sysfb.width * sysfb.height) {
                return -1;
        }
        return 0;
}

void kputchar_to(
                        /* unicode character */
                        unsigned short int c,
                        /* cursor position on screen in characters */
                        uint32_t row, uint32_t column,
                        /* foreground and background colors */
                        uint32_t fg, uint32_t bg,
                        /* character attributes */
                        uint32_t attr)
{
        if ((sysfb.video_initialized == false) ||
            (row >= text.rows) || (column >= text.columns)) {
                return;
        }
        /* cast the address to PSF header struct */
        font = (PSF_font *) & _font_start;
        uint32_t scanline = sysfb.width * sysfb.bpp / 32;       /*usually bpp=32 */
        /* we need to know how many bytes encode one row */
        int bytesperline = (font->width + 7) / 8;
        /* unicode translation */
        if (unicode != NULL) {
                c = unicode[c];
        }
        /* get the glyph for the character. If there's no
           glyph for a given character, we'll display the first glyph. */
        unsigned char *glyph =
            (unsigned char *)&_font_start +
            font->headersize +
            (c > 0 && c < (font->numglyph) ? c : 0) * font->bytesperglyph;
        /* calculate the upper left corner on screen where 
         * we want to display.*/
        auto int offs =
            (row * font->height * sysfb.pitch / 4) +
            (column * (font->width + 1) * sysfb.bpp / 32);
        /* finally display pixels according to the bitmap */
        register uint32_t x, y, line, mask;
        for (y = 0; y < font->height; y++) {
                /* save the starting position of the line */
                line = offs;
                mask = 1 << (font->width - 1);
                /* display a row */
                for (x = 0; x < font->width; x++) {
                        register uint32_t *volatile vaddr =
                            (uint32_t *) sysfb.virtaddr + line;
                        if (verify_addr(vaddr) == 0) {
                                if (attr == TRANSPARENT) {
                                        *vaddr |=
                                            ((uint32_t) *
                                             glyph) & (mask) ? fg : bg;
                                } else {
                                        *vaddr =
                                            ((uint32_t) *
                                             glyph) & (mask) ? fg : bg;
                                }
                        } else {
                                return; /* out of bounds */
                        }
                        /* adjust to the next pixel */
                        mask >>= 1;
                        line += 1;
                }
                /* adjust to the next line */
                glyph += bytesperline;
                offs += scanline;
        }
}

#endif                          /* USE_VESA */

void print_video_info(void)
{
        kprintf("Resolution: %dx%d\n", sysfb.width, sysfb.height);
        kprintf("chars per row: %d\n", text.columns);
        kprintf("chars per column: %d\n", text.rows);
        kprintf("font width: %d\n", font->width);
        kprintf("font height :%d\n", font->height);
}
