;Copyright (C) 2018  Igor Muravyov <igor.muravyov.2015@gmail.com>
bits 64
extern kputstr_32
extern start_kernel
extern boot_magic
extern boot_info
extern stack_top

global early_kernel_init
GREEN equ 0x2
; kernel x64 port early initialization
section .rodata
verify_x64: db " Long mode initialized", 0
section .text
early_kernel_init:
        ;LONG MODE STACK REINITIALIZATION
        mov rsp, stack_top
        mov rax, verify_x64
        mov cl, GREEN
        call kputstr_32
        mov rdi,  [boot_magic] ; EAX <-- boot magic (via multiboot spec)
        mov rsi,  [boot_info]  ; EBX <-- boot info
.start_kernel:
        call start_kernel
        jmp .start_kernel
