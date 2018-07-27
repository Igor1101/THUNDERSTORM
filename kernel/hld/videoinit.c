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
#include <memmapping.h>

uint32_t scanline;
uint32_t bytesperline;

/**
 * bool init_video(void)
 * @return: if false then we coudn`t initialize it;
 */
bool init_video(void)
  /* without this function 
   * access to videomemory can corrupt kernel
   */
{
        if (sysfb.is_initialized == false) {
                sysfb.video_initialized = false;
                return false;
        }
        /*processing font */
        if (font_info() != 0) { /*system font must be compatible !!! */
                sysfb.video_initialized = false;
                return false;
        }
#ifdef USE_VESA
        sysfb.video_initialized = true;
        /* low level map this address
         * to somewhere in kernel memory */
        sysfb.virtaddr = map_video((volatile void *)sysfb.addr);
#endif
        /* init vars */
        scanline = sysfb.width * sysfb.bpp /32;
        /* bytes encode one line */
        bytesperline = (font -> width + 7) / 8;
        /* unicode translation */
        return sysfb.video_initialized;
}
