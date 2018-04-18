#ifndef SYSVARS
#define SYSVARS




extern struct RAM_INFO
{
  /* highest RAM info, loaded from bootloader */
  uintptr_t* highest;
  /* lowest RAM info, loaded from bootloader */
  uintptr_t* lowest;
} RAM;



#endif /* sysvars */
