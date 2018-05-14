#ifndef SYSVARS
#define SYSVARS


#ifdef __cplusplus
extern "C" {
#endif

#include <TH/lld.h>
#ifndef MAX_RAM_ENTRIES 
  #define MAX_RAM_ENTRIES 100 /*max amount of 
                                 ram entries */
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
/* amount of ram entries currently stored */
int ram_entries;

/* video info */
extern struct Text_mode_pointer text;
extern struct Framebuffer sysfb;

void select_RAM(void);

#ifdef __cplusplus
}
#endif

#endif /* sysvars */
