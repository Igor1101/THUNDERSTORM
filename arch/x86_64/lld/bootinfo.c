#define LOADER_NAME 2

#include <stdint.h>
#include <kstdio.h>
/**
 * bootinfo() function x86_64 port
 * recognizes computer system info via multiboot2 spec 
 * */
void* loader_name(void *ebx)
{
  kprintf("\nloader is %s", (char*) (ebx + 2 * sizeof (uint32_t)));
  return ( ebx + *( (uint32_t*) (ebx + sizeof (uint32_t) ) ) );
}
void bootinfo(void * ebx)
{
  /* total size of header*/
  uint32_t header_size=*((uint32_t*)ebx);
  void * start_ebx=ebx;
  kprintf("\ntotal size=%x",header_size);
  /* reserved */
  ebx += sizeof (uint32_t);
  /* next type */
  ebx += sizeof (uint32_t);
  /* parsing header info */
  while(ebx <= ( start_ebx + header_size ) )
  {
    switch(*(uint32_t*)ebx)
    {
      case LOADER_NAME: ebx = loader_name(ebx); break;
    }
    ebx += sizeof (uint32_t);
  }
}
