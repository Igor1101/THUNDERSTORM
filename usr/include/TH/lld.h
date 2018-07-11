/*
 * Copyright (C) 2018  Igor Muravyov <igor.muravyov.2015@gmail.com>
 */
#ifndef LLD_TH
#define LLD_TH

#include <gcc_opt.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#if defined USE_VGA || defined NO_VIDEOMODE
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
                Default = 2,
                DefaultBG = Black
        };
#endif                          /*USE_VGA */

        enum char_attributes {
                /*
                 * for kputchar_to func
                 */
                TRANSPARENT,
                NOTRANSPARENT
        };

#ifdef USE_VESA
        enum Color {
                Black = 0,
                Blue = 0x041B36,
                Green = 0x1A3604,
                Cyan = 0x4B81C0,
                Red = 0xFF0000,
                Magenta = 0xFF00FF,
                Brown = 0xA52A2A,
                LightGray = 0x8F959B,
                DarkGray = 0x52575C,
                LightBlue = 0x88B9F2,
                LightGreen = 0x88F2AF,
                LightCyan = 0xC9FFE5,
                LightRed = 0xFF001A,
                Pink = 0xFFC0CB,
                Yellow = 0xFFFF00,
                White = 0xFFFFFF,
                Default = 0x2A7225,
                DefaultBG = Black
        };
#endif                          /*USE_VESA */

        struct Text_mode_pointer
/* suitable with VGA and VESA */
        {
                bool is_initialized;
                uint32_t row;
                uint32_t col;
                uint32_t bgcolor;
                uint32_t fgcolor;
                uint32_t rows;  /* in chars */
                uint32_t columns;       /* in chars */
        };

/* video framebuffer */
        struct Framebuffer {
                bool is_initialized;    /* 
                                         *  used by bootinfo() to make sure, 
                                         *  option found in bootloader structure
                                         *  */
                bool video_initialized; /* if(is_initialized==false) video_initialized
                                         * must be also false */
                uintptr_t *virtaddr;
                uintptr_t *addr;
                uint32_t pitch;
                uint32_t width;
                uint32_t height;
                uint32_t bpp;
                uint32_t type;
                uint32_t colorinfo;
                uint32_t cursor_enabled;
        };

        void tui_init(void);
        void select_fgcolor(int color);
        void select_bgcolor(int color);

/* VIDEO */
        void kputpixel(uint32_t x, uint32_t y, uint32_t color);
        void print_video_info(void);

/* return true if op successful */
        bool init_video(void);
        int font_info(void);
#ifndef NO_VIDEOMODE
        void enable_cursor(uint8_t cursor_start, uint8_t cursor_end);
        uint32_t determine_columns(void);
        uint32_t determine_rows(void);
        void update_cursor(int row, int col);
        void make_newline(void);
        void kputchar_to(
                                /* unicode character */
                                unsigned short int c,
                                /* cursor position on screen in characters  */
                                uint32_t row, uint32_t column,
                                /* foreground and background colors */
                                uint32_t fg, uint32_t bg,
                                /* character attributes */
                                uint32_t attr);
#endif /* NO_VIDEOMODE */

/* miscelaneous */
/* cpu management */
        void kpause(void);      /* for debug */

#ifdef NO_VIDEOMODE
        /* videomode funcs stubs */
        FORCE_INLINE void kputchar_to(
                                /* unicode character */
                                unsigned short int c,
                                /* cursor position on screen in characters  */
                                uint32_t row, uint32_t column,
                                /* foreground and background colors */
                                uint32_t fg, uint32_t bg,
                                /* character attributes */
                                uint32_t attr)
        {
                (void)c;
                (void)row;
                (void)column;
                (void)fg;
                (void)bg;
                (void)attr;
        }

        FORCE_INLINE void make_newline(void){};

        FORCE_INLINE void update_cursor(int row, int col)
        {
                (void)row;
                (void)col;
        }
        
        FORCE_INLINE void enable_cursor(uint8_t cursor_start, uint8_t cursor_end)
        {
                (void)cursor_start;
                (void)cursor_end;
        }
        FORCE_INLINE uint32_t determine_columns(void)
        {
                return 0;
        }
        FORCE_INLINE uint32_t determine_rows(void)
        {
                return 0;
        }

#endif /* NO_VIDEOMODE */
#ifdef __cplusplus
}
#endif
#endif                          /* LLD_TH */
