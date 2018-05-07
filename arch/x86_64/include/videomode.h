#include <stdbool.h>
#define VIRTUAL_VIDEO_ADDR 0x3fe00000

bool video_initialized=false;

void map_video(void*addr);
