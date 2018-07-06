/*
 * Copyright (C) 2018  Igor Muravyov <igor.muravyov.2015@gmail.com>
 */
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
	if (sysfb.is_initialized == false) {
		sysfb.video_initialized = false;
		return false;
	}
	/*processing font */
	if (font_info() != 0) {	/*system font must be compatible !!! */
		sysfb.video_initialized = false;
		return false;
	}
#ifdef USE_VESA
	sysfb.video_initialized = true;
	/* low level map this address
	 * to somewhere in kernel memory */
	sysfb.virtaddr = map_video((volatile void *)sysfb.addr);
#endif
	return sysfb.video_initialized;
}
