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

static void text_select_fgcolor(int color);
static void text_select_bgcolor(int color);
static void text_putchar(int chr);

struct Text_mode_pointer text = (struct Text_mode_pointer){
       /* prevent from crashing due to NULL pointer */
       .select_fgcolor = text_select_bgcolor,
       .select_bgcolor = text_select_fgcolor,
       .putchar = text_putchar
};
 static void putchar_to(/* unicode character */
                 unsigned short int c,
                         uint32_t row, uint32_t column,
                         uint32_t fg, uint32_t bg,
                         uint32_t attr)
{
         (void)c;
         (void)row;
         (void)column;
         (void)fg;
         (void)bg;
         (void)attr;
 }
static void make_newline(void){};
 static void clear_screen(void){};
static void cursor_update(text_t row, text_t col) 
 {
         (void)row;
         (void)col;
 }
static void cursor_enable(text_t cursor_start, text_t cursor_end) 
{                                            
         (void)cursor_start;
         (void)cursor_end;
}
static uint32_t determine_columns(void) { return 0;} 
 static uint32_t determine_rows(void) { return 0; }
static void char_invert(text_t row, text_t column)
{
        (void)row;
        (void)column;
}

static void char_copy(
       /* cursor position on screen in characters
        * for destination */
        text_t d_row, text_t d_column,
        text_t s_row, text_t s_column
       )
{
        (void)d_row;
        (void)s_row; 
        (void)d_column;
        (void)s_column;
}
void update_screen(void){}
int verify_addr(text_t*addr){(void)*addr;return 0;}
static void print_video_info(void) { }

struct Framebuffer sysfb = {
        .print_video_info = print_video_info,
        .verify_addr = verify_addr,
        .clear_screen = clear_screen,
        .putchar_to = putchar_to,
        .update_screen = update_screen,
        .char_invert = char_invert,
        .char_copy = char_copy,
        .cursor_enable = cursor_enable,
        .cursor_update = cursor_update,
        .make_newline = make_newline,
        .determine_rows = determine_rows,
        .determine_columns = determine_columns
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
