#ifdef KDEBUG

void kpause(void)
{
  asm volatile (" mov $10000000, %rdx");
  asm volatile (" d: dec %rdx");
  asm volatile (" jnz d");
}

#endif
