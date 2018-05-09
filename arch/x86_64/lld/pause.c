
void kpause(void)
{
  asm volatile (" mov $100000000, %rdx");
  asm volatile (" d: dec %rdx");
  asm volatile (" jnz d");
}

