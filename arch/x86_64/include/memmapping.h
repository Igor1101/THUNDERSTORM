/*
 * Copyright (C) 2018  Igor Muravyov <igor.muravyov.2015@gmail.com>
 */
#ifndef VIDEO_INIT_H
#define VIDEO_INIT_H

/**
 * void* last_addr(void)
 * return: last mapped kernel address
 */
void* last_addr(void);

uintptr_t *map_video(volatile void *addr);

#endif                          /* VIDEO_INIT_H */
