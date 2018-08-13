/*
 * Copyright (C) 2018  Igor Muravyov <igor.muravyov.2015@gmail.com>
 */
#include <kstdio.h>
#include <kstdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <TH/lld.h>
#include <TH/sysvars.h>
#include <TH/font.h>
#include <asm/memory_mapping.h>

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
        printk("Mmaping memory\t");
        sysfb.virtaddr = kmmap((void *)sysfb.addr, 
                        sysfb.width * sysfb.height * sysfb.bpp / 4);
        if(sysfb.virtaddr == NULL ) {
                kputs("[FALSE]");
                sysfb.video_initialized = false;
                return false;
        } else {
                kputs("[TRUE]");
        }
        /* init copy address 
         * if kcalloc is unsuccessfull (NULL), we`ll use old method */
        sysfb.copy = kcalloc(sysfb.width * sysfb.height * sysfb.bpp / 4);
        printk("Video allocating buffer\t");
        if(sysfb.copy != NULL) {
                kputs("[OK]");
        } else {
                kputs("[FALSE]");
        }

#endif
        /* init vars */
        scanline = sysfb.width * sysfb.bpp /32;
        /* bytes encode one line */
        bytesperline = (font -> width + 7) / 8;
        /* unicode translation */
        return sysfb.video_initialized;
}
