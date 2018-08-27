/*
 * Copyright (C) 2018  Igor Muravyov <igor.muravyov.2015@gmail.com>
 */
#ifndef LLD_TH
#define LLD_TH

#ifdef __cplusplus
extern "C" {
#endif

#include <compiler_opt.h>
#include <stdint.h>
#include <stdbool.h>


#if defined USE_VGA 
        enum Color {
                Black = 0,
                Blue = 1,
                Green = 2,
                Cyan = 3,
                Red = 4,
                Magenta = 5,
                Brown = 6,
                LightGray = 7,
                DarkGray = 8,
                LightBlue = 9,
                LightGreen = 10,
                LightCyan = 11,
                LightRed = 12,
                Pink = 13,
                Yellow = 14,
                White = 15,
                DefaultFG = Green,
                DefaultBG = Black,
                DefaultLogFG = Green
        };

        static FORCE_INLINE void fb_clear_screen(void){};
#else /* use standard color pallete */
        enum Color {
                Black           = 0,
                Blue            = 0x6495ED,/* CornflowerBlue */
                Green           = 0x008000,
                Cyan            = 0x00FFFF,
                Red             = 0xFF0000,
                Magenta         = 0xFF00FF,
                Brown           = 0xA52A2A,
                LightGray       = 0xD3D3D3,
                DarkGray        = 0xA9A9A9,
                LightBlue       = 0xADD8E6,
                LightGreen      = 0x90EE90,
                LightCyan       = 0xE0FFFF,
                LightRed        = 0xFF0000,
                Pink            = 0xFFC0CB,
                Yellow          = 0xFFFF00,
                White           = 0xFFFFFF,
                DefaultFG       = 0x9E839D,
                DefaultBG       = 0x130815,
                DefaultLogFG    = Green
        };


#endif                          /*USE_VGA */

        enum char_attributes {
                /*
                 * for kputchar_to func
                 */
                TRANSPARENT,
                NOTRANSPARENT
        };


/* suitable with VGA and VESA */
        typedef uint32_t text_t;
        struct Text_mode_pointer {
                bool is_initialized;
                text_t row;
                text_t col;
                text_t bgcolor;
                text_t fgcolor;
                text_t rows;  /* amount of rows in chars */
                text_t columns;       /* in chars */
                bool cursor_not_clear; /* (true) will not clear cursor one time */
                text_t lines_offset;
                void (*select_fgcolor)(int color);
                void (*select_bgcolor)(int color);
                void (*putchar)(int chr);
        };

/* video framebuffer */
        struct Framebuffer {
                bool is_initialized;    /* 
                                         *  used by bootinfo() to make sure, 
                                         *  option found in bootloader structure
                                         *  */
                bool video_initialized; /* if(is_initialized==false) video_initialized
                                         * must be also false */
                void *virtaddr;
                /* physical */
                void *addr;
                /* copy in kernel memory */
                void *copy;
                uint32_t pitch;
                uint32_t width;
                uint32_t height;
                uint32_t bpp;
                uint32_t type;
                uint32_t colorinfo;
                bool cursor_enabled;
                void (*print_video_info)(void);
                /* determines does this virtual 
                 * addr belong to
                 * framebuffer display
                 */
                int (*verify_addr)(uint32_t* addr);
                void (*clear_screen)(void);
                void (*putchar_to)(
                        /* unicode character */
                        unsigned short int c,
                        /* cursor position on screen in characters */
                        uint32_t row, uint32_t column,
                        /* foreground and background colors */
                        uint32_t fg, uint32_t bg,
                        /* character attributes */
                        uint32_t attr);
                void (*update_screen)(void);
                void (*char_invert)(text_t row, text_t column);
                void (*char_copy)(
                        /* cursor position on screen in characters
                         * for destination */
                        text_t d_row, text_t d_column,
                        text_t s_row, text_t s_column
                );
                void (*cursor_enable)(text_t cursor_start, text_t cursor_end);
                void (*cursor_update)(text_t row, text_t col);
                void (*make_newline)(void);
                text_t (*determine_rows)(void);
                text_t (*determine_columns)(void);
        };


        void tui_init(text_t lines_offset);

/* VIDEO */
        void kputpixel(uint32_t x, uint32_t y, uint32_t color);

/* return true if op successful */
        bool init_video(void);
        int font_info(void);

/* miscelaneous */
/* cpu management */
        void kpause(void);      /* for debug */

#ifdef NO_VIDEOMODE
        FORCE_INLINE void fb_init(void)
        {
        }
#else
        void fb_init(void);
#endif

#ifdef __cplusplus
}
#endif

#endif                          /* LLD_TH */
