/*
 * Copyright (C) 2018-2019  Igor Muravyov <igor.muravyov.2015@gmail.com>
 */
#ifndef VGA_H
#define VGA_H

#define VGAADDR (volatile void* volatile)0xb8000
#define COLUMNS 80
#define ROWS 23
/* The I/O ports */
#define FB_COMMAND_PORT         0x3D4
#define FB_DATA_PORT            0x3D5

/* The I/O port commands */
#define FB_HIGH_BYTE_COMMAND    14
#define FB_LOW_BYTE_COMMAND     15

#endif                          /* VGA_H */
