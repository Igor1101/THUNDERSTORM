#include <stdint.h>
#include <kstdio.h>
#include <memory_mapping.h>
#include <TH/lld.h>
#include <TH/sysvars.h>
void init_paging(void);
uintptr_t* map_video(void* addr)
{/*
    used for mapping videoaddr/framebuffer
    to kernel memory
    virtual addr = 2MB * (PG_SIZE - 8)/4 = 0x3fe00000;
    */
  uint64_t videoaddr = (uint64_t)addr | 0b10000011;
  uint64_t* volatile map_addr = (void*)&p2_table + PG_SIZE - sizeof(uint64_t);
  (*map_addr) = videoaddr;
  init_paging();
  kpause(); /* <--- Wait for MMU, to change vaddr */
  return (uintptr_t*) 
    ( (uintptr_t)(2 << 20) * (PG_SIZE / 8 - 1 ) );
}

