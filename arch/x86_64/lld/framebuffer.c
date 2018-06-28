/* 
 * This driver is universal for a huge amount
 * of platforms, but it is slow
 */
#include <kstdio.h>
#include <TH/lld.h>
#include <TH/sysvars.h>
#include <TH/font.h>
#include <video_lld.h>
#include <stdbool.h>
#include <kstring.h>
#include <gcc_opt.h>
bool video_initialized = false;
bool cursor_enabled = false;

LIKELY void kputpixel(uint32_t x, uint32_t y, uint32_t color)
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

#ifdef USE_VESA
UNLIKELY uint32_t determine_rows(void)
{
  /* system font should already be processed
   * */
  int row = 1;
  while((row * font -> height) < sysfb.height)
  {
    row++;
  }
  return row - 1;
}

UNLIKELY uint32_t determine_columns(void)
{
  /* system font should already be processed
   * */
  uint32_t column = 1;
  while(column * (font -> width + 1) * sysfb.bpp / 32 < sysfb.width)
  {
    column++;
  }
  return column - 1;
}


void enable_cursor(uint8_t cursor_start, uint8_t cursor_end)
{
  /* getting rid of compiler warnings */
  volatile uint8_t s=cursor_start; s=cursor_end;s--;
  cursor_enabled = true;
}
LIKELY void make_newline(void)
{
  if(video_initialized == false)
    return;
  void* end = sysfb.virtaddr + 
    (1 * font -> height * sysfb.pitch / 8);
  kmemcpy_ptr(sysfb.virtaddr, end, sysfb.width * sysfb.height * sysfb.bpp / 8);
}

LIKELY void update_cursor(int row, int col)
{
  //static int lastrow;
  //static int lastcol;
  if(cursor_enabled == true)
  {
    kputchar_to('_', row, col, Cyan, Black, TRANSPARENT);
  }
  //lastrow = row;
  //lastcol = col;
}

#endif
