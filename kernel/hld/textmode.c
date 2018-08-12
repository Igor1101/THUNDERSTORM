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
struct Text_mode_pointer text;
void tui_init(text_t lines_offset)
{
        text.lines_offset = lines_offset;
        text.row = BEGINNING + text.lines_offset;
        text.col = BEGINNING;
        text.fgcolor = DefaultFG;
        text.bgcolor = DefaultBG;
        enable_cursor(BEGINNING, text.rows);
        text.columns = determine_columns();
        text.rows = determine_rows();
        text.is_initialized = true;
}

void select_fgcolor(int color)
{
        text.fgcolor = color;
}

void select_bgcolor(int color)
{
        text.bgcolor = color;
}

void newline(void)
{
        if (text.row >= text.rows - 1) {
#ifdef USE_VESA
                if(sysfb.copy == NULL) 
                        invert_char(text.row, text.col);
#endif /* USE_VESA */
                make_newline();
                text.col = BEGINNING;
                text.cursor_not_clear = true;
                return;
        }
        text.row++;
        text.col = BEGINNING;
}

void kputchar(int8_t chr)
{
#ifdef USE_SERIAL
        /* even if tui not initialized, 
         * it is possible to write to serial port through tui
         */
        serial_write_asyn(SERIAL_MAIN, chr);
        if (chr == '\n')
                serial_write_asyn(SERIAL_MAIN, '\r');
#endif
        if (text.is_initialized == false)
                return;

        switch (chr) {
        case '\n':
                newline();
                update_cursor(text.row, text.col);
                return;
        case '\r':
                text.col = BEGINNING;
                update_cursor(text.row, text.col + 1);
                return;
        case '\b':
                text.col--;
                update_cursor(text.row, text.col + 1);
                return;
        case '\t':
                text.col += text.col % 2 + 2;/* now tab=2spaces */
                update_cursor(text.row, text.col + 1);
                return;
        default:
                break;
        }
        update_cursor(text.row, text.col + 1);
        kputchar_to(chr, text.row, text.col,
                    text.fgcolor, text.bgcolor, NOTRANSPARENT);
        if (text.col >= text.columns && text.row < text.rows) {
                newline();
        } else if (text.row >= text.rows && text.col >= text.columns) {
                make_newline();
                text.col = BEGINNING;
                text.row = text.rows - 1;
        } else {
                text.col++;
        }
}
