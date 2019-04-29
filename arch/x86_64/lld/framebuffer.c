/* 
 * Copyright (C) 2018-2019  Igor Muravyov <igor.muravyov.2015@gmail.com>
 * This driver is universal for a huge amount
 * of platforms, but it is slow
 */
#include <kstdio.h>
#include <TH/lld.h>
#include <TH/sysvars.h>
#include <TH/font.h>
#include <stdbool.h>
#include <kstring.h>
#include <compiler_opt.h>

LIKELY void kputpixel(uint32_t x, uint32_t y, uint32_t color)
{
        uint8_t* max_addr;
        register uint8_t *vaddr;
        if (sysfb.video_initialized == false)
                return;
        if(sysfb.copy == NULL) {
                max_addr = (uint8_t *) sysfb.virtaddr +
                sysfb.width * sysfb.height * sysfb.bpp / 8;
                vaddr = (uint8_t *) sysfb.virtaddr
                + y * sysfb.pitch + x * sysfb.bpp / 8;
        } else {
                max_addr = (uint8_t *) sysfb.copy +
                sysfb.width * sysfb.height * sysfb.bpp / 8;
                vaddr = (uint8_t *) sysfb.copy
                + y * sysfb.pitch + x * sysfb.bpp / 8;
        }
        /* don`t let it out of bounds */
        if (vaddr >= max_addr)
                return;
        *vaddr = color & 255;   /* BLUE */
        *(vaddr + 1) = (color >> 8) & 255;      /* GREEN */
        *(vaddr + 2) = (color >> 16) & 255;     /* RED */
}

/*
void krectangle(uint32_t x, uint32_t y, 
    uint32_t width, uint32_t height, uint32_t RGB)
{
  
}*/

