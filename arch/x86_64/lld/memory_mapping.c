#include <stdint.h>
#include <kstdio.h>
#include <memory_mapping.h>
#include <TH/lld.h>
#include <TH/sysvars.h>
void init_paging(void);
uintptr_t* map_video(volatile void* addr)
{/*
    in arch/x86_64/boot/boot.asm: 8 * 2MB
    used for mapping videoaddr/framebuffer
    to kernel memory
    virtual addr = 2MB * (PG_SIZE - 8)/4 = 0x3fe00000;
  */
  uint64_t volatile videoaddr = (uint64_t)addr | 0b10000011;
  volatile uint64_t* volatile map_addr = (void*)&p2_table + PG_SIZE - 8 * sizeof(uint64_t);
  (*map_addr) = videoaddr; // 1st 2 MB`s : 1
  videoaddr += 2<<20;
  (*(map_addr + 1)) = videoaddr; // second 2 MB`s : 2
  videoaddr += 2<<20;
  (*(map_addr + 2) ) = videoaddr; // : 3
  videoaddr += 2<<20;
  (*(map_addr + 3) ) = videoaddr; // : 4
  videoaddr += 2<<20;
  (*(map_addr + 4) ) = videoaddr; // : 5
  videoaddr += 2<<20;
  (*(map_addr + 5) ) = videoaddr; // : 6
  videoaddr += 2<<20;
  (*(map_addr + 6) ) = videoaddr; // : 7
  videoaddr += 2<<20;
  (*(map_addr + 7) ) = videoaddr; // : 8

  init_paging();
  kpause(); /* <--- Wait for MMU, to change vaddr */
  return (uintptr_t*) 
    ( (uintptr_t)(2 << 20) * (PG_SIZE / 8 - 8 ) );
}

