/*
 * Copyright (C) 2018  Igor Muravyov <igor.muravyov.2015@gmail.com>
 */
#ifndef CONVERT_H
#define CONVERT_H

#include <stdint.h>

/*
 * These all functions are about to 
 * convert endiannes
 */
int64_t swp_i64(int64_t val);
uint64_t swp_u64(uint64_t val);
uint16_t swp_u16(uint16_t val);
int16_t swp_i16(int16_t val);
uint32_t swp_u32(uint32_t val);
int32_t swp_i32(int32_t val);

#endif				/* CONVERT_H */
