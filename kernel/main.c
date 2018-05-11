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
#include <TH/sysvars.h>
#include <kstdlib.h>
#include <kstdio.h>
#include <stdbool.h>

/* declared vars */
struct RAM_INFO RAM = 
{
  NULL,
  NULL
};
struct Framebuffer sysfb;
struct RAM_MAP ram_map[MAX_RAM_ENTRIES];

int main(void* pcinfo)
{
  tui_init();
  static char verifier=100;
  if(verifier != 100)
  {
    return (1);/* GOT is not working, 
                  smth wrong with bss */
  }
  kputs("\n\n\nTHUNDERSTORM Embedded system\n\
 COPYRIGHT Igor Muravyov 2018");
#ifdef KDEBUG
  select_fgcolor(Cyan);
  kputs("This is a DEBUG version of kernel,\
output is too slow");
#endif /* KDEBUG */

#ifdef USE_VGA
  tui_init();
  select_fgcolor(Red);
  kputs("Note, that VGA mode is Legacy, used only for debbuging");
  select_fgcolor(Default);
#endif /* USE_VGA */

  kputs("COMPUTER INFO:");
  bootinfo(pcinfo);
  if(init_video() == true)
  {
    tui_init();
    kputs("videomode successfully started");
    select_fgcolor(Cyan);
    select_bgcolor(Blue);
    kputs(BIG_SYS_EMBLEM);
    select_bgcolor(Black);
    select_fgcolor(Default);
  }
  cpu_halt();
  while(1);
}
