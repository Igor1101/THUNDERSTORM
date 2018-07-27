/*
 * Copyright (C) 2018  Igor Muravyov <igor.muravyov.2015@gmail.com>
 */
#include <gcc_opt.h>
#include <kstdio.h>
#include <kstring.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <TH/lld.h>
#include <TH/sysvars.h>
#include <TH/font.h>

#ifdef USE_VESA

LIKELY int verify_addr(uint32_t * addr)
  /* verify if we are writing to permissible address */
{
        if (addr >= (uint32_t *) sysfb.virtaddr + sysfb.width * sysfb.height) {
                return -1;
        }
        return 0;
}

/* This function has been copied from 
 * wiki.OSdev.org
 */
LIKELY void kputchar_to(
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
        /* unicode translation */
        /* Now useless
        if (unicode != NULL) {
                c = unicode[c];
        }*/
        /* get the glyph for the character. If there's no
           glyph for a given character, we'll display the first glyph. */
        unsigned char *glyph =
            (unsigned char *)&_font_start +
            font->headersize +
            (c > 0 && c < (font->numglyph) ? c : 0) * font->bytesperglyph;
        /* calculate the upper left corner on screen where 
         * we want to display.*/
        register int offs =
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


LIKELY void invert_char(uint32_t row, uint32_t column)
{
        if ((sysfb.video_initialized == false) ||
            (row >= text.rows) || (column >= text.columns)) {
                return;
        }
        /* get the glyph for the character. If there's no
           glyph for a given character, we'll display the first glyph. */
        unsigned char *glyph =
            (unsigned char *)&_font_start +
            font->headersize;
        /* calculate the upper left corner on screen where 
         * we want to display.*/
        register int offs =
            (row * font->height * sysfb.pitch / 4) +
            (column * (font->width + 1) * sysfb.bpp / 32);
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
                                        *vaddr = ~ ( *vaddr );
                                }
                        else {
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

LIKELY void copy_char(
                        /* cursor position on screen in characters 
                         * for destination */
                        uint32_t d_row, uint32_t d_column,
                        uint32_t s_row, uint32_t s_column
                )
{
        if ((sysfb.video_initialized == false) ||
            (d_row >= text.rows) || (d_column >= text.columns) ||
            (s_row >= text.rows) || (s_column >= text.columns)
            ) {
                return;
        }
        /* calculate the upper left corner on screen where 
         * we want to display.*/
        register uint32_t d_offs =
            (d_row * font->height * sysfb.pitch / 4) +
            (d_column * (font->width + 1) * sysfb.bpp / 32);
        register uint32_t s_offs =
            (s_row * font->height * sysfb.pitch / 4) +
            (s_column * (font->width + 1) * sysfb.bpp / 32);
        /* finally display pixels according to the bitmap */
        register uint32_t x, y, d_line, s_line;
        for (y = 0; y < font->height; y++) {
                /* save the starting position of the line */
                d_line = d_offs;
                s_line = s_offs;
                /* display a row */
                for (x = 0; x < font->width; x++) {
                        *( (uint32_t *) sysfb.virtaddr + d_line ) = 
                                    *( (uint32_t *) sysfb.virtaddr + s_line);
                        /* adjust to the next pixel */
                        d_line ++;
                        s_line ++;
                }
                /* adjust to the next line */
                s_offs += scanline;
                d_offs += scanline;
        }
}

UNLIKELY uint32_t determine_rows(void)
{
        /* system font should already be processed
         * */
        int row = 1;
        while ((row * font->height) < sysfb.height) {
                row++;
        }
        return row;
}

UNLIKELY uint32_t determine_columns(void)
{
        /* system font should already be processed
         * */
        uint32_t column = 1;
        while (column * (font->width + 1) * sysfb.bpp / 32 < sysfb.width) {
                column++;
        }
        return column - 1;
}

UNLIKELY void enable_cursor(uint8_t cursor_start, uint8_t cursor_end)
{
        /* getting rid of compiler warnings */
        (void)cursor_start;
        (void)cursor_end;
        sysfb.cursor_enabled = true;
}


LIKELY void make_newline(void)
{
        if (sysfb.video_initialized == false)
                return;
        for(
                        text_t dline=text.lines_offset, 
                        sline=text.lines_offset+1;
                        dline<text.rows;
                        dline++, sline++){
                for(
                                text_t dcol=0, scol=0;
                                dcol<=text.columns;
                                dcol++, scol++) {
                        copy_char(dline, dcol, sline, scol);
                }
        }
        /* clearing last line */
        for(text_t col=0; col<=text.columns; col++) {
                kputchar_to(0, text.rows - 1, col, 
                                DefaultBG, DefaultBG, NOTRANSPARENT);
        }
}

/*deprecated but may be still useful
LIKELY void make_newline(void)
{
        if (sysfb.video_initialized == false)
                return;
        uintptr_t offset = 
                text.lines_offset * font-> height * sysfb.pitch / 8;
        void *src = 
                sysfb.virtaddr + (1 * font->height * sysfb.pitch / 8) + offset;
        kmemcpy_ptr(sysfb.virtaddr + offset, src,
                    sysfb.width * sysfb.height * sysfb.bpp / 8 
                    - 1 * (font -> height * sysfb.pitch / 8) );
}*/

LIKELY void update_cursor(int row, int col)
{
        static int oldrow;
        static int oldcol;
        if (sysfb.cursor_enabled == true) {
                invert_char(row, col);
        }
        if( (!text.cursor_not_clear) && (sysfb.cursor_enabled) ){
                invert_char(oldrow, oldcol);
        }
        text.cursor_not_clear = false;
        oldrow = row;
        oldcol = col;
}

#endif                          /* USE_VESA */

UNLIKELY void print_video_info(void)
{
        kprintf("Resolution: %dx%d\n", sysfb.width, sysfb.height);
        kprintf("chars per row: %d\n", text.columns);
        kprintf("chars per column: %d\n", text.rows);
        kprintf("font width: %d\n", font->width);
        kprintf("font height :%d\n", font->height);
}
