/*
 * Copyright (C) 2018  Igor Muravyov <igor.muravyov.2015@gmail.com>
 */
#include <compiler_opt.h>
#include <kstdio.h>
#include <kstring.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <TH/lld.h>
#include <TH/sysvars.h>
#include <TH/font.h>

#ifdef CONFIG_USE_VESA

/*
 * fb initialization
 */

static LIKELY int fb_verify_addr(uint32_t * addr);
static LIKELY void fb_clear_screen(void);
static LIKELY void fb_putchar_to(
                        /* unicode character */
                        unsigned short int c,
                        /* cursor position on screen in characters */
                        uint32_t row, uint32_t column,
                        /* foreground and background colors */
                        uint32_t fg, uint32_t bg,
                        /* character attributes */
                        uint32_t attr);
static LIKELY void fb_display_update(void);
static LIKELY void fb_char_invert(text_t row, text_t column);
static UNLIKELY text_t fb_determine_rows(void);
static UNLIKELY text_t fb_determine_columns(void);
static UNLIKELY void fb_cursor_enable(text_t cursor_start, text_t cursor_end);
static LIKELY void fb_make_newline(void);
static LIKELY void fb_cursor_update(text_t row, text_t col);
static UNLIKELY void fb_print_video_info(void);
static LIKELY void fb_char_copy(
                        /* cursor position on screen in characters 
                         * for destination */
                        text_t d_row, text_t d_column,
                        text_t s_row, text_t s_column
                );

/* 
 * make an object of ... class sysfb ;) 
 * */
void fb_init(void)
{
        sysfb.print_video_info = fb_print_video_info;
        sysfb.verify_addr = fb_verify_addr;
        sysfb.clear_screen = fb_clear_screen;
        sysfb.putchar_to = fb_putchar_to;
        sysfb.update_screen = fb_display_update;
        sysfb.char_invert = fb_char_invert;
        sysfb.char_copy = fb_char_copy;
        sysfb.make_newline = fb_make_newline;
        sysfb.cursor_enable = fb_cursor_enable;
        sysfb.cursor_update = fb_cursor_update;
        sysfb.make_newline = fb_make_newline;
        sysfb.determine_rows = fb_determine_rows;
        sysfb.determine_columns = fb_determine_columns;
}
static LIKELY int fb_verify_addr(uint32_t * addr)
  /* verify if we are writing to permissible address */
{
        if (addr >= (uint32_t *) sysfb.virtaddr + sysfb.width * sysfb.height) {
                return -1;
        }
        return 0;
}

/* TODO : optimize clear screen function: remove putpixel here */
static LIKELY void fb_clear_screen(void)
{
        for(uint32_t y = 0; y < sysfb.height; y++) {
                for(uint32_t x = 0; x < sysfb.width; x++) {
                        kputpixel(x, y , DefaultBG);
                }
        }
        fb_display_update();
}

static LIKELY void fb_putchar_to(
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
                register uint32_t *volatile vaddr;
                register uint32_t *volatile copy;
                for (x = 0; x < font->width + 1; x++) {
                        vaddr = (uint32_t *) sysfb.virtaddr + line;
                        copy = (uint32_t*) sysfb.copy + line;
                        if (fb_verify_addr(vaddr) == 0) {
                                if (attr == TRANSPARENT) {
                                        *vaddr |=
                                            ((uint32_t) *
                                             glyph) & (mask) ? fg : bg;
                                        if(sysfb.copy != NULL) {
                                                *copy |= 
                                                        ((uint32_t) * glyph)
                                                        & (mask) ? fg : bg;
                                        }

                                } else {
                                        *vaddr =
                                            ((uint32_t) *
                                             glyph) & (mask) ? fg : bg;
                                        if(sysfb.copy != NULL) {
                                                *copy = 
                                                        ((uint32_t) * glyph) 
                                                        & (mask) ? fg : bg;
                                        }
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

static LIKELY void fb_display_update(void)
{
        uintptr_t offset = text.lines_offset * 
                font -> height * sysfb.pitch ;
        if(sysfb.copy == NULL)
                return;
        kmemcpy_ptr(
                        sysfb.virtaddr + offset, 
                        sysfb.copy + offset, 
                    sysfb.width * sysfb.height * sysfb.bpp / 8 
                    );
}


static LIKELY void fb_char_invert(text_t row, text_t column)
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
                register uint32_t *volatile vaddr;
                //register uint32_t *volatile copy;
                for (x = 0; x < font->width; x++) {
                        vaddr = (uint32_t *) sysfb.virtaddr + line;
                        //copy = (uint32_t *) sysfb.copy + line;
                        if (fb_verify_addr(vaddr) == 0) {
                                        *vaddr = ~ ( *vaddr );
                                        /*
                                        if(sysfb.copy != NULL) {
                                                *copy = ~ ( *copy);
                                        }*/
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

static LIKELY void fb_char_copy(
                        /* cursor position on screen in characters 
                         * for destination */
                        text_t d_row, text_t d_column,
                        text_t s_row, text_t s_column
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
                uint32_t *virtaddr;
                virtaddr = (uint32_t*)sysfb.virtaddr;
                for (x = 0; x < font->width; x++) {
                        *( virtaddr + d_line ) = 
                                    *( virtaddr + s_line);
                        /* adjust to the next pixel */
                        d_line ++;
                        s_line ++;
                }
                /* adjust to the next line */
                s_offs += scanline;
                d_offs += scanline;
        }
}

static UNLIKELY text_t fb_determine_rows(void)
{
        /* system font should already be processed
         * */
        int row = 1;
        while ((row * font->height) < sysfb.height) {
                row++;
        }
        return row - 1;
}

static UNLIKELY text_t fb_determine_columns(void)
{
        /* system font should already be processed
         * */
        uint32_t column = 1;
        while (column * (font->width + 1) * sysfb.bpp / 32 < sysfb.width) {
                column++;
        }
        return column - 1;
}

static UNLIKELY void fb_cursor_enable(text_t cursor_start, text_t cursor_end)
{
        /* getting rid of compiler warnings */
        (void)cursor_start;
        (void)cursor_end;
        sysfb.cursor_enabled = true;
}


/*
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
        for(text_t col=0; col<=text.columns; col++) {
                kputchar_to(0, text.rows - 1, col, 
                                DefaultBG, DefaultBG, NOTRANSPARENT);
        }
}*/

static LIKELY void fb_make_newline(void)
{
        if (sysfb.video_initialized == false)
                return;
        uintptr_t offset = 
                text.lines_offset * 
                font -> height * sysfb.pitch ;
        void *src, *dest;
        if(sysfb.copy == NULL) {
                src = 
                        sysfb.virtaddr + 
                        (font->height * sysfb.pitch ) +
                        offset;

                dest = sysfb.virtaddr + offset;
        }
        else {
                src = 
                        sysfb.copy + 
                        (font->height * sysfb.pitch ) +
                        offset;
                dest = sysfb.copy + offset;
        }
        kmemcpy_ptr(dest, src,
                    sysfb.width * sysfb.height * sysfb.bpp / 8 
                    - 1 * (font -> height * sysfb.pitch / 8) );
        for(text_t col=0; col<=text.columns; col++) {
                fb_putchar_to(0, text.rows - 1, col, 
                                DefaultBG, DefaultBG, NOTRANSPARENT);
        }
        fb_display_update();
}

static LIKELY void fb_cursor_update(text_t row, text_t col)
{
        static int oldrow;
        static int oldcol;
        if (sysfb.cursor_enabled == true) {
                fb_char_invert(row, col);
        }
        if( (!text.cursor_not_clear) && (sysfb.cursor_enabled) ){
                fb_char_invert(oldrow, oldcol);
        }
        text.cursor_not_clear = false;
        oldrow = row;
        oldcol = col;
}

#endif                          /* CONFIG_USE_VESA */

UNLIKELY void fb_print_video_info(void)
{
        kprintf("Resolution: %dx%d\n", sysfb.width, sysfb.height);
        kprintf("chars per row: %d\n", text.columns);
        kprintf("chars per column: %d\n", text.rows);
        kprintf("font width: %d\n", font->width);
        kprintf("font height :%d\n", font->height);
}
