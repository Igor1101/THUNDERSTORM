#include <stdint.h>
#include <stdbool.h>
#include <gcc_opt.h>
/*
 * Copyed from: osdev.org
 * --------------------------------------------------------------------------
 * CMSIS-LIKE INTERFACE FOR CPU MANAGEMENT
 */
FORCE_INLINE void io_wait(void)
{
    /* Port 0x80 is used for 'checkpoints' during POST. */
    /* The Linux kernel seems to think it is free for use :-/ */
    asm volatile ( "outb %%al, $0x80" : : "a"(0) );
}

/* Input a byte from a port */
FORCE_INLINE unsigned char inb(uint16_t port)
{
  io_wait();
  volatile unsigned char ret;
  asm volatile ("inb %%dx,%%al":"=a" (ret):"d" (port));
  return ret;
}

/* Output a byte to a port */

FORCE_INLINE void outb(uint16_t port, uint8_t value)
{
  io_wait();
  asm volatile ("outb %%al,%%dx": :"d" (port), "a" (value));
}

FORCE_INLINE bool are_interrupts_enabled()
{
    unsigned long flags;
    asm volatile ( "pushf\n\t"
                   "pop %0"
                   : "=g"(flags) );
    return flags & (1 << 9);
}

/* 
 * Read the current value of the CPU's 
 * time-stamp counter and store into EDX:EAX
 */
FORCE_INLINE uint64_t rdtsc(void)
{
    uint32_t low, high;
    asm volatile("rdtsc":"=a"(low),"=d"(high));
    return ((uint64_t)high << 32) | low;
}

/*
 * Read the value in a control register.
 */
FORCE_INLINE unsigned long read_cr0(void)
{
    unsigned long val;
    asm volatile ( "mov %%cr0, %0" : "=r"(val) );
    return val;
}

/*
 * write msr
 */
FORCE_INLINE static void wrmsr(uint64_t msr, uint64_t value)
{
	uint32_t low = value & 0xFFFFFFFF;
	uint32_t high = value >> 32;
	asm volatile (
		"wrmsr"
		:
		: "c"(msr), "a"(low), "d"(high)
	);
}

/* 
 * read msr
 */
FORCE_INLINE uint64_t rdmsr(uint64_t msr)
{
	uint32_t low, high;
	asm volatile (
		"rdmsr"
		: "=a"(low), "=d"(high)
		: "c"(msr)
	);
	return ((uint64_t)high << 32) | low;
}

/* 
 * define new interrupt table
 */
FORCE_INLINE void lidt(void* base, uint16_t size)
{/*This function works in 32 and 64bit mode*/
  struct 
  {
    uint16_t length;
    void*    base;
  } PACKED IDTR = { size, base };
  asm ( "lidt %0" : : "m"(IDTR) );  /* let the compiler 
                                     * choose an addressing mode*/
}
