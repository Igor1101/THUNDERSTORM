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

#ifdef USE_VGA
  tui_init();
  select_fgcolor(Red);
  kputs("Note, that VGA mode is Legacy, used only for debbuging");
  select_fgcolor(Default);
#endif /* USE_VGA */

  static char verifier=100;
  if(verifier != 100)
  {
    return (1);/* GOT is not working, 
                  smth wrong with bss */
  }
#ifdef KDEBUG
  select_fgcolor(Cyan);
  kputs("This is a DEBUG version of kernel,\
output is too slow");
#endif /* KDEBUG */


  kputs("COMPUTER INFO:");
  bootinfo(pcinfo);
  if(init_video() == true)
  {
    tui_init();
    kputs("videomode successfully started");
    print_video_info();
    /* verifying bounds of display*/
    kputchar_to('A', text.rows - 1, text.columns - 1, Red, Red);
    kputchar_to('A', 0, text.columns - 1, Red, Red);
    kputchar_to('A', text.rows - 1, 0, Red, Red);
    select_fgcolor(Cyan);
    select_bgcolor(Blue);
    kputs(SMALL_SYS_EMBLEM);
    select_bgcolor(Black);
    select_fgcolor(Default);
  }
  kprintf("\n\n\nTHUNDERSTORM %s Embedded system\n\
 COPYRIGHT Igor Muravyov (c) %s ", RELEASE, YEARS);
  select_RAM();
  cpu_halt();
  while(1);
}
