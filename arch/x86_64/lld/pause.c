
void kpause(void)
{
  asm volatile (" mov $80000000, %rdx");
  asm volatile (" d: dec %rdx");
  asm volatile (" jnz d");
}

