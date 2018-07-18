/*
 * Copyright (C) 2018  Igor Muravyov <igor.muravyov.2015@gmail.com>
 */
#ifndef FONT_H
#define FONT_H

#ifdef __cplusplus
extern "C" {
#endif

#define PSF_FONT_MAGIC 0x864ab572
#define USHRT_MAX 512

        typedef struct {
                uint32_t magic; /* magic bytes to identify PSF */
                uint32_t version;       /* zero */
                uint32_t headersize;    /* offset of bitmaps in file, 32 */
                uint32_t flags; /* 0 if there's no unicode table */
                uint32_t numglyph;      /* number of glyphs */
                uint32_t bytesperglyph; /* size of each glyph */
                uint32_t height;        /* height in pixels */
                uint32_t width; /* width in pixels */
        } PSF_font;
        uint16_t font_arr[USHRT_MAX];
        uint16_t *unicode;

        extern char _font_start;        /* < ------------ font in rodata */
        extern char _font_end;

        extern PSF_font *font;

        extern uint32_t scanline;
        extern uint32_t bytesperline;

#ifdef __cplusplus
}
#endif
#endif                          /* FONT_H */
