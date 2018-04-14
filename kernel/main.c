/* High level TH kernel initialization */
/*
 _____ _   _ _   _ _   _ ____  _____ ____  ____ _____ ___  ____  __  __        __
|_   _| | | | | | | \ | |  _ \| ____|  _ \/ ___|_   _/ _ \|  _ \|  \/  |  __  /  \_  _
  | | | |_| | | | |  \| | | | |  _| | |_) \___ \ | || | | | |_) | |\/| |_/  \/     \/ \
  | | |  _  | |_| | |\  | |_| | |___|  _ < ___) || || |_| |  _ <| |  | |\_ ___________/
  |_| |_| |_|\___/|_| \_|____/|_____|_| \_\____/ |_| \___/|_| \_\_|  |_|   /
                                                                          /\_
                                                                          \  \
__________________________________________________________________________/___________
*/
#include <TH/lld.h>
#include <TH/sysinfo.h>
#include <kstdlib.h>
#include <kstdio.h>
int main()
{
  static char verifier=100;
  if(verifier != 100)
  {
    return (1);/* GOT is not working, 
                  smth wrong with bss */
  }
  tui_init(Cyan);
  kputs((int8_t*)"\n\n\nTHUNDERSTORM Embedded system\n\
 COPYRIGHT Igor Muravyov 2018");
  kputs((int8_t*)SMALL_SYS_EMBLEM);
  kprintf("DEC: %dHEX: 0x%x", 255, 255);
  asm("hlt");
  while(1);
}
