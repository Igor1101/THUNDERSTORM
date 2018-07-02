/*
 * Copyright (C) 2018  Igor Muravyov <igor.muravyov.2015@gmail.com>
 */

void kpause(void)
{
  asm volatile (" push %rdx");
  asm volatile (" mov $80000000, %rdx");
  asm volatile (" d: dec %rdx");
  asm volatile (" jnz d");
  asm volatile (" pop %rdx");
}

