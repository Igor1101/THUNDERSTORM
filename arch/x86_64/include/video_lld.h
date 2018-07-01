#ifndef VIDEO_INIT_H
#define VIDEO_INIT_H
#include <stdbool.h>

bool video_initialized;

uintptr_t* map_video(volatile void* addr);

#endif /* VIDEO_INIT_H */
