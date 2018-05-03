#ifndef SYSVARS
#define SYSVARS
#include <TH/lld.h>



extern struct RAM_INFO
{
  /* highest RAM info, loaded from bootloader */
  uintptr_t* highest;
  /* lowest RAM info, loaded from bootloader */
  uintptr_t* lowest;
} RAM;

/* video info */
extern struct Framebuffer sysfb;


#endif /* sysvars */
