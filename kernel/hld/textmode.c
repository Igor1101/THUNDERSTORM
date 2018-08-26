/* 
 * Copyright (C) 2018  Igor Muravyov <igor.muravyov.2015@gmail.com> 
 * THUNDERSTORM VGA HL DRIVER *
 */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <TH/lld.h>
#include <TH/sysvars.h>
#include <asm/serial.h>
#define BEGINNING 0

static void do_nothing(int color)
{
       (void)color;
}

struct Text_mode_pointer text = (struct Text_mode_pointer){
       /* prevent from crashing due to NULL pointer */
       .select_fgcolor = do_nothing,
       .select_bgcolor = do_nothing,
       .putchar = (void(*)(int chr))do_nothing
};

static void text_select_fgcolor(int color);
static void text_select_bgcolor(int color);
static void text_putchar(int chr);

void tui_init(text_t lines_offset)
{
        fb_init();
        text.lines_offset = lines_offset;
        text.row = BEGINNING + text.lines_offset;
        text.col = BEGINNING;
        text.fgcolor = DefaultFG;
        text.bgcolor = DefaultBG;
        sysfb.cursor_enable(BEGINNING, text.rows);
        text.columns = sysfb.determine_columns();
        text.rows = sysfb.determine_rows();
        text.is_initialized = true;
        text.select_fgcolor = text_select_fgcolor;
        text.select_bgcolor = text_select_bgcolor;
        text.putchar = text_putchar;
        /* clearing screen */
        sysfb.clear_screen();
}

static void text_select_fgcolor(int color)
{
        text.fgcolor = color;
}

static void text_select_bgcolor(int color)
{
        text.bgcolor = color;
}

static void newline(void)
{
        if (text.row >= text.rows - 1) {
#ifdef USE_VESA
                if(sysfb.copy == NULL) 
                        sysfb.char_invert(text.row, text.col);
#endif /* USE_VESA */
                sysfb.make_newline();
                text.col = BEGINNING;
                text.cursor_not_clear = true;
                return;
        }
        text.row++;
        text.col = BEGINNING;
}

static void text_putchar(int chr)
{
#ifdef USE_SERIAL
        /* even if tui not initialized, 
         * it is possible to write to serial port through tui
         */
        chr = (int8_t)chr;
        serial_write_asyn(SERIAL_MAIN, chr);
        if (chr == '\n')
                serial_write_asyn(SERIAL_MAIN, '\r');
#endif
        if (text.is_initialized == false)
                return;

        switch (chr) {
        case '\n':
                newline();
                sysfb.cursor_update(text.row, text.col);
                return;
        case '\r':
                text.col = BEGINNING;
                sysfb.cursor_update(text.row, text.col + 1);
                return;
        case '\b':
                text.col--;
                sysfb.cursor_update(text.row, text.col + 1);
                return;
        case '\t':
                text.col += text.col % 2 + 2;/* now tab=2spaces */
                sysfb.cursor_update(text.row, text.col + 1);
                return;
        default:
                break;
        }
        sysfb.cursor_update(text.row, text.col + 1);
        sysfb.putchar_to(chr, text.row, text.col,
                    text.fgcolor, text.bgcolor, NOTRANSPARENT);
        if (text.col >= text.columns && text.row < text.rows) {
                newline();
        } else if (text.row >= text.rows && text.col >= text.columns) {
                sysfb.make_newline();
                text.col = BEGINNING;
                text.row = text.rows - 1;
        } else {
                text.col++;
        }
}
