#include <kstdio.h>
#include <TH/lld.h>
#include <TH/sysvars.h>
#include <videomode.h>
#include <stdbool.h>
bool video_initialized = false;
void kputpixel(uint32_t x, uint32_t y, uint32_t color)
{
  if(video_initialized == false)
    return;
  register uint8_t* where = (uint8_t*)sysfb.virtaddr
    + y * sysfb.pitch + x * sysfb.bpp / 8;
  *where = color & 255; /* BLUE */
  *(where + 1)= (color >> 8) & 255; /* GREEN */
  *(where + 2)= (color >> 16) & 255; /* RED */
}

void init_video(void)
{
#ifdef USE_VBE
  video_initialized = true;
  map_video(sysfb.addr);
#endif
}


