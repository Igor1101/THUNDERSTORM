/* TH kernel initialization */
#include <TH/lld.h>
int main()
{
  static long int jk;
  jk=0x1122334455667788;
  if(jk!=0x1122334455667788)
  {
    return (-1);/*GOT is not working */
  }
  vga_init(Cyan);
  kputchar_to('?', 0, 0, Green);
  kputchar('+');
  kputchar('+');
  kputchar('+');
  kputchar('+');
  kputchar('+');
  asm("hlt");
  while(1);
}
