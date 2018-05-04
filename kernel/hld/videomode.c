#include <kstdio.h>
#include <TH/lld.h>
#include <TH/sysvars.h>
void kputpixel(uint32_t x, uint32_t y, uint32_t color)
{
  register uint8_t* where = (uint8_t*)sysfb.addr 
    + y * sysfb.pitch + x * sysfb.bpp;
 *where = color & 255; /* BLUE */
 *(where + 1)= (color >> 8) & 255; /* BLUE */
 *(where + 2)= (color >> 16) & 255; /* BLUE */
}


