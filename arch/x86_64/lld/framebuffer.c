#include <kstdio.h>
#include <TH/lld.h>
#include <TH/sysvars.h>
#include <TH/font.h>
#include <video_lld.h>
#include <stdbool.h>

bool video_initialized = false;

void kputpixel(uint32_t x, uint32_t y, uint32_t color)
{
  if(video_initialized == false)
    return;
  register uint8_t* vaddr= (uint8_t*)sysfb.virtaddr
    + y * sysfb.pitch + x * sysfb.bpp / 8;
  /* dont let it out of bounds */
  if(vaddr >= (uint8_t*) sysfb.virtaddr + 
      sysfb.width * sysfb.height * sysfb.bpp / 8)
    return;
  *vaddr = color & 255; /* BLUE */
  *(vaddr + 1)= (color >> 8) & 255; /* GREEN */
  *(vaddr + 2)= (color >> 16) & 255; /* RED */
}
/*
void krectangle(uint32_t x, uint32_t y, 
    uint32_t width, uint32_t height, uint32_t RGB)
{
  
}*/
#ifdef USE_VBE
uint32_t determine_columns(void)
{
  /* system font should already be processed
   * */
  return sysfb.width / (font -> width + 1) - font -> width + 7;
}

uint32_t determine_rows(void)
{
  /* system font should already be processed
   * */
  return sysfb.height / font -> height - 7 ;
}


void enable_cursor(uint8_t cursor_start, uint8_t cursor_end)
{volatile uint8_t s=cursor_start; s=cursor_end;s--;}
void make_newline(void){}
void update_cursor(int x, int y)
{int s=x;s=y;s++;}
#endif
