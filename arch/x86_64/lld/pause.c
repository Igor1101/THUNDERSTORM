#ifdef KDEBUG

void kpause(void)
{
  asm volatile (" mov $30000000, %rdx");
  asm volatile (" d: dec %rdx");
  asm volatile (" jnz d");
}

#endif
