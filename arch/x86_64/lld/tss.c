#include <x86_64/tss.h>

#define x64_LOW(X) ( (uint64_t) (X) & 0xFFFF )
#define x64_MIDDLE(X) ( (uint64_t) (X) >> 16 & 0xFFFF )
#define x64_HIGH(X) ( (uint64_t) (X) >> 32 )
/*
void set_tss_desc(void)
{
        tss_table = {
                .limit          =0xFFFF,
                .low_base       =
                */
