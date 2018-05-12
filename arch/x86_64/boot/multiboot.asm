; Multiboot header
MAGIC equ 0xe85250d6
ARCH  equ 0 ;/* i386 */
section .multiboot
boot_start:
  dd MAGIC
  dd ARCH
  dd boot_end - boot_start
  dd 0x100000000 - (MAGIC + ARCH + (boot_end - boot_start))
address_tag_start:
%ifdef USE_VBE
framebuffer_tag_start:
  dw 5 ; 
  dw 1 ; optional
  dd .end - framebuffer_tag_start ; size
  dd 0  ;width
  dd 0 ;heiht
  dd 32 ;bpp
.end:
%endif
tags_end:
; terminate tags:
;  dw 0
  dw 0
  dd 8
boot_end:
