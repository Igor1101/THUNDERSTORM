#include <kstdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <TH/lld.h>
#include <TH/sysvars.h>
#include <videomode.h>
#include "videotext.h"
/* number of bytes in each line, it's possible it's not screen width * bytesperpixel! */
/* import our font that's in the object file we've created above */
asm("_font_start: \n"
    " .incbin \"../../usr/share/consolefonts/uni.psf\"\n"
    "_font_end: \n"
    );
extern char _font_start;
extern char _font_end;
uint16_t *unicode = NULL;
void font_info(void)
{
  select_color(Cyan);
  kputs("fonts info:");
  PSF_font *font = (PSF_font*)&_font_start;
  if(PSF_FONT_MAGIC == font -> magic)
  {
    kputs("font is PSF");
  }
  kprintf("version %d\n", font -> version);
  kprintf("headersize %d\n", font -> headersize);
  kprintf("flags %d\n", font -> flags);
  kprintf("numglyph %d\n", font -> numglyph);
  kprintf("height %d\n", font -> height);
  kprintf("width %d\n", font -> width);
  select_color(Default);
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
    return;
  uint32_t scanline = sysfb.width * sysfb.bpp / 8;
    /* cast the address to PSF header struct */
    PSF_font *font = (PSF_font*)&_font_start;
    /* we need to know how many bytes encode one row */
    int bytesperline=(font -> width + 7) / 8;
    /* unicode translation */
    if(unicode != NULL) {
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
    int offs =
        (cy * font->height * scanline) +
        (cx * (font->width+1) * 4);
    /* finally display pixels according to the bitmap */
    uint32_t x,y, line,mask;
    for(y=0;y<font->height;y++)
    {
        /* save the starting position of the line */
        line=offs;
        mask=1<<(font->width-1);
        /* display a row */
        for(x=0;x<font->width;x++)
        {
            *( (uint32_t*)(sysfb.virtaddr + line / 8) ) = ((int)*glyph) & (mask) ? fg : bg;
            /* adjust to the next pixel */
            mask >>= 1;
            line += 4;
        }
        /* adjust to the next line */
        glyph += bytesperline;
        offs  += scanline;
    }
}

