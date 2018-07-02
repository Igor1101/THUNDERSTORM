; MULTIBOOT2 HEADER
MAGIC equ 0xe85250d6
ARCH  equ 0 ;/* i386 */
section .multiboot
multiboot_start:
  dd MAGIC
  dd ARCH
  dd multiboot_end - multiboot_start
  dd 0x100000000 - (MAGIC + ARCH + (multiboot_end - multiboot_start))
address_tag_start:
%ifdef USE_VESA
framebuffer_tag_start:
  dw 5 ; 
  dw 1 ; optional
  dd .end - framebuffer_tag_start ; size
  dd 0  ;width
  dd 0 ;heiht
  dd 0 ;bpp
.end:
%endif
tags_end:
; terminate tags:
;  dw 0
  dw 0
  dd 8
multiboot_end:
