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
  dw 0 ; optional
  dd .end - framebuffer_tag_start ; size
  dd 1024
  dd 768
  dd 32
.end:
%endif
tags_end:
; terminate tags:
;  dw 0
  dw 0
  dd 8
boot_end:
