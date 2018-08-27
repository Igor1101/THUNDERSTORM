/*
 * Copyright (C) 2018  Igor Muravyov <igor.muravyov.2015@gmail.com>
 * THUNDERSTORM VGA LL DRIVER 
 * Note, that VGA 0xb8000 text support is provided 
 * for debbuging only purposes
 * */

#ifdef USE_VGA
#include <compiler_opt.h>
#include <stdint.h>
#include <kstring.h>
#include <TH/lld.h>
#include <TH/sysvars.h>
#include <x86_64/cpu_management.h>
#include <x86_64/VGA.h>

static uint32_t vga_determine_columns(void);
static uint32_t vga_determine_rows(void);
static void vga_update_cursor(text_t row, text_t col);
static void vga_enable_cursor(text_t cursor_start, text_t cursor_end);
static void vga_putchar_to(
                        /* unicode character */
                        unsigned short int c,
                        /* cursor position on screen in characters  */
                        uint32_t cx, uint32_t cy,
                        /* foreground and background colors */
                        uint32_t fg, uint32_t bg,
                        /* useless here */
                        uint32_t attr);
static void vga_make_newline(void);

void fb_init(void)
{
        sysfb.putchar_to = vga_putchar_to;
        sysfb.make_newline = vga_make_newline;
        sysfb.cursor_enable = vga_enable_cursor;
        sysfb.cursor_update = vga_update_cursor;
        sysfb.determine_rows = vga_determine_rows;
        sysfb.determine_columns = vga_determine_columns;
}

static uint32_t vga_determine_columns(void)
{
        /* these functions added to provide portability,
           encapsulation */
        return COLUMNS;
}

static uint32_t vga_determine_rows(void)
{
        /* these functions added to provide portability,
           encapsulation */
        return ROWS;
}

static void vga_update_cursor(text_t row, text_t col)
{
        uint16_t offset = (row * COLUMNS) + col;
        outb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
        outb(FB_DATA_PORT, ((offset >> 8) & 0x00FF));
        outb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
        outb(FB_DATA_PORT, offset & 0x00FF);
}

static void vga_enable_cursor(text_t cursor_start, text_t cursor_end)
{
        outb(FB_COMMAND_PORT, 0x0A);
        outb(FB_DATA_PORT, (inb(FB_DATA_PORT) & 0xC0) | cursor_start);
        outb(FB_COMMAND_PORT, 0x0B);
        //outb(FB_DATA_PORT, (inb(0x3E0) & 0xE0) | cursor_end);
        (void)cursor_end;
}

static void vga_putchar_to(
                        /* unicode character */
                        unsigned short int c,
                        /* cursor position on screen in characters  */
                        uint32_t cx, uint32_t cy,
                        /* foreground and background colors */
                        uint32_t fg, uint32_t bg,
                        /* useless here */
                        uint32_t attr)
{                               /*bgcolor - useless var */
        (void)attr;
        bg++;
        register volatile int16_t *vga_addr = VGAADDR + 2 * (cx * 80 + cy);
        *vga_addr = (uint16_t) c | ((uint16_t) fg) << 8;
}

static void vga_make_newline(void)
{
#ifdef DEBUG
        //kpause();
#endif
        memmove((void *)VGAADDR, (void *)VGAADDR + 160,
               ROWS * COLUMNS * 2 + COLUMNS * 2);
}

#endif
