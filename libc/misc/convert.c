#include <stdint.h>

/*
 * Copyright (C) 2018  Igor Muravyov <igor.muravyov.2015@gmail.com>
 * These all functions are about to 
 * convert endiannes
 */
int64_t swp_i64(int64_t val)
{				/* convert big endian to little endian
				   and vice versa
				 */
	val = ((val << 8) & 0xFF00FF00FF00FF00ULL)
	    | ((val >> 8) & 0x00FF00FF00FF00FFULL);
	val = ((val << 16) & 0xFFFF0000FFFF0000ULL)
	    | ((val >> 16) & 0x0000FFFF0000FFFFULL);
	return (val << 32) | ((val >> 32) & 0xFFFFFFFFULL);
}

uint64_t swp_u64(uint64_t val)
{
	val = ((val << 8) & 0xFF00FF00FF00FF00ULL)
	    | ((val >> 8) & 0x00FF00FF00FF00FFULL);
	val = ((val << 16) & 0xFFFF0000FFFF0000ULL)
	    | ((val >> 16) & 0x0000FFFF0000FFFFULL);
	return (val << 32) | (val >> 32);
}

uint16_t swp_u16(uint16_t val)
{
	return (val << 8) | (val >> 8);
}

int16_t swp_i16(int16_t val)
{
	return (val << 8) | ((val >> 8) & 0xFF);
}

uint32_t swp_u32(uint32_t val)
{
	val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF);
	return (val << 16) | (val >> 16);
}

int32_t swp_i32(int32_t val)
{
	val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF);
	return (val << 16) | ((val >> 16) & 0xFFFF);
}
