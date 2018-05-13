#include <kstdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <TH/lld.h>
#include <TH/sysvars.h>
#include <video_lld.h>

bool init_video(void)
  /* without this function 
   * access to videomemory can corrupt kernel
   */
{
  /*processing font */
  if(font_info() != 0)
  {/*system font must be compatible !!! */
    video_initialized = false;
    return false;
  }
#ifdef USE_VBE
  video_initialized = true;
  /* low level map this address
   * to somewhere in kernel memory */
  sysfb.virtaddr = map_video((volatile void* )sysfb.addr);
#endif
  return video_initialized;
}

