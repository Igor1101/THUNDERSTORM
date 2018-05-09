/* this files path 
 * kernel/hld/videotext.c
 * should not be changed since it use`s relative links
 */
#include <kstdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <TH/lld.h>
#include <TH/sysvars.h>
#include <video_lld.h>
#include "videotext.h"

/* 
 * import psf(PC screen font) , 
 * only psf2 fonts are currently supported (not psfu )
 * */
asm("_font_start: \n"
    " .incbin \"../../usr/share/consolefonts/uni.psf\"\n"
    "_font_end: \n"
    );

extern char _font_start;
extern char _font_end;
uint16_t *unicode = NULL;

int font_info(void)
{
  int ret = 0;
  select_color(Cyan);
  kputs("fonts info:");
  PSF_font *font = (PSF_font*)&_font_start;
  if(PSF_FONT_MAGIC == font -> magic)
  {
    kputs("font is PSF");
  }
  else
  {
    kputs("Warning: font is not supported by kernel");
    ret = -1;
  }
  kprintf("version %d\n", font -> version);
  kprintf("headersize %d\n", font -> headersize);
  kprintf("flags %d\n", font -> flags);
  kprintf("numglyph %d\n", font -> numglyph);
  kprintf("height %d\n", font -> height);
  kprintf("width %d\n", font -> width);
  select_color(Default);
  return ret;
}

int verify_addr(uint32_t *addr)
  /* verify if we are writing to permissible address */
{
  if(addr >= (uint32_t*) sysfb.virtaddr + 
      sysfb.width * sysfb.height)
  {
    return -1;
  }
  return 0;
}

void putchar(
    /* note that this is int, not char as it's a unicode character */
    unsigned short int c,
    /* cursor position on screen, in characters not in pixels */
    int cx, int cy,
    /* foreground and background colors, say 0xFFFFFF and 0x000000 */
    uint32_t fg, uint32_t bg)
{
  if(video_initialized == false) 
  {
    return;
  }
    /* cast the address to PSF header struct */
    PSF_font *font = (PSF_font*)&_font_start;
    uint32_t scanline = sysfb.width * sysfb.bpp / 32/*usually bpp=32 */;
    /* we need to know how many bytes encode one row */
    int bytesperline=(font -> width + 7) / 8;
    /* unicode translation */
    if(unicode != NULL) 
    {
        c = unicode[c];
    }
    /* get the glyph for the character. If there's no
       glyph for a given character, we'll display the first glyph. */
    unsigned char *glyph =
     (unsigned char*)&_font_start +
     font->headersize +
     (c>0&&c<font-> numglyph?c:0)*font->bytesperglyph;
    /* calculate the upper left corner on screen where we want to display.
       we only do this once, and adjust the offset later. This is faster. */
    auto int offs =
        (cy * font->height * sysfb.pitch / 4) +
        (cx * (font->width + 1) * sysfb.bpp / 32);
    /* finally display pixels according to the bitmap */
    register uint32_t x,y, line,mask;
    for(y=0;y<font->height;y++)
    {
        /* save the starting position of the line */
        line = offs;
        mask = 1<<(font->width - 1);
        /* display a row */
        for(x=0;x<font->width;x++)
        {
          register uint32_t* volatile vaddr = (uint32_t*)sysfb.virtaddr + line;
          if(verify_addr(vaddr) == 0)
          {
            *vaddr  = ((uint32_t)*glyph) & (mask) ? fg : bg;
          }
            /* adjust to the next pixel */
            mask >>= 1;
            line += 1;
        }
        /* adjust to the next line */
        glyph += bytesperline;
        offs  += scanline;
    }
}


