#define LOADER_NAME 2
#define MEMORY 4

#include <stdint.h>
#include <kstdio.h>
#include <TH/sysvars.h>
/**
 * bootinfo() function x86_64 port
 * recognizes computer system info via multiboot2 spec 
 * */
void* loader_name(void *ebx)
{
  static int already_set=0;
  if(already_set == 1)
  {
    return ebx;
  }
  already_set=1;
  kprintf("\nloader is %s", (char*) (ebx + 2 * sizeof (uint32_t)));
  /* find out size of structure */
  return ( ebx + 
      *( (uint32_t*) (ebx + sizeof (uint32_t) ) ) 
      - sizeof (uint32_t) );
}

void* memory(void *ebx)
{
  kprintf("\nRAM LOWESTLIMIT: 0x%xk HIGHEST: 0x%xk", 
*(uint32_t*) (ebx + 2 * sizeof (uint32_t) ),
*(uint32_t*) (ebx + 3 * sizeof (uint32_t) )
      );
  return ( ebx + 
      *( (uint32_t*) (ebx + sizeof (uint32_t) ) ) 
      - sizeof (uint32_t) );
}

void bootinfo(void * ebx)
{
  /* total size of header*/
  uint32_t header_size=*((uint32_t*)ebx);
  void * start_ebx=ebx;
  kprintf("\n\n\ntotal size=%x",header_size);
  /* reserved */
  ebx += sizeof (uint32_t);
  /* next type */
  ebx += sizeof (uint32_t);
  /* parsing header info */
  while(ebx <= ( start_ebx + header_size ) )
  {
    ebx += sizeof (uint32_t);
    switch(*(uint32_t*)ebx)
    {
      case MEMORY: ebx = memory(ebx); break;
      case LOADER_NAME: ebx = loader_name(ebx); break;
    }
  }
}
