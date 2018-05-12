
void kpause(void)
{
  asm volatile (" push %rdx");
  asm volatile (" mov $100000, %rdx");
  asm volatile (" d: dec %rdx");
  asm volatile (" jnz d");
  asm volatile (" pop %rdx");
}

