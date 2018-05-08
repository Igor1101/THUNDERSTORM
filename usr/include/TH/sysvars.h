#ifndef SYSVARS
#define SYSVARS

#include <TH/lld.h>
#ifndef MAX_RAM_ENTRIES 
  #define MAX_RAM_ENTRIES 100 /*max amount of 
                                information about ram entries */
#endif /* RAM_ENTRIES */
/*ram types */

#define PHYSRAM 1
#define ACPIRAM 3
#define PRESERVED 4
extern struct RAM_INFO
{
  /* highest RAM info, loaded from bootloader */
  uintptr_t* highest;
  /* lowest RAM info, loaded from bootloader */
  uintptr_t* lowest;
} RAM;
extern struct RAM_MAP
{
  uintptr_t* base_addr;
  uintptr_t length;
  uint32_t type;
} ram_map[MAX_RAM_ENTRIES];
int ram_entries;
/* video info */
extern struct Framebuffer sysfb;


#endif /* sysvars */
