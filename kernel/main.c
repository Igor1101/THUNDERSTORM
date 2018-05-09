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

/* declared vars */
struct RAM_INFO RAM = 
{
  NULL,
  NULL
};
struct Framebuffer sysfb;
struct RAM_MAP ram_map[MAX_RAM_ENTRIES];
void putchar(
    /* note that this is int, not char as it's a unicode character */
    unsigned short int c,
    /* cursor position on screen, in characters not in pixels */
    int cx, int cy,
    /* foreground and background colors, say 0xFFFFFF and 0x000000 */
    uint32_t fg, uint32_t bg);

int main(void* sysinfo)
{
  static char verifier=100;
  if(verifier != 100)
  {
    return (1);/* GOT is not working, 
                  smth wrong with bss */
  }
  tui_init(Default);
  kputs("\n\n\nTHUNDERSTORM Embedded system\n\
 COPYRIGHT Igor Muravyov 2018");
#ifdef KDEBUG
  select_color(Cyan);
  kputs("This is a DEBUG version of kernel,\
output is too slow");
#endif /* KDEBUG */

#ifdef USE_VGA
  select_color(Red);
  kputs("Note, that VGA mode is Legacy, used only for debbuging");
  select_color(Default);
#endif /* USE_VGA */
  kputs("COMPUTER INFO:");
  bootinfo(sysinfo);
  init_video();
  for(uint32_t i=0;i<1024;i++)
  {
    for(int j=0; j<200 ;j++)
    {
      //kputpixel(j,i, 255<<8);
      putchar(j+ 40,j,i, 255<< 8, 0);
    }
  }
  cpu_halt();
  while(1);
}
