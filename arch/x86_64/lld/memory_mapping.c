#include <stdint.h>
#include <kstdio.h>
#include <memory_mapping.h>

void init_paging(void);
void map_video(void* addr)
{/*
    used for mapping videoaddr/framebuffer
    to kernel memory
    virtual addr = 2MB * (PG_SIZE - 8)/4 = 0x3fe00000;
    */
  uint64_t videoaddr = (uint64_t)addr | 0b10000011;
  uint64_t* volatile map_addr = (void*)&p2_table + PG_SIZE - sizeof(uint64_t);
  (*map_addr) = videoaddr;
  init_paging();
}

