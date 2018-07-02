bits 64
global kernel_init
extern kputstr_to
extern start_kernel
extern boot_magic
extern boot_info
GREEN equ 0x2
; kernel x64 port early initialization
section .rodata
  verify_x64: db " Long mode initialized", 0
section .text
kernel_init:
  mov rax, 0x1122334455667788
  mov rax, verify_x64
  mov cl, GREEN
  call kputstr_to
  mov rdi,  [boot_magic] ; EAX <-- boot magic (via multiboot spec)
  mov rsi,  [boot_info]  ; EBX <-- boot info
  call start_kernel
  hlt
