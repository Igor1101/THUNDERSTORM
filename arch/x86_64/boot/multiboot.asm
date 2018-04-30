; Multiboot header
MAGIC equ 0xe85250d6
ARCH  equ 0 ;/* i386 */
section .multiboot
boot_start:
  dd MAGIC
  dd ARCH
  dd boot_end - boot_start
  dd 0x100000000 - (MAGIC + ARCH + (boot_end - boot_start))
; tags:

  dw 0
  dw 0
  dd 8
; EGA console:
  dw 4
  dw 0
  dd 12
;
boot_end:
