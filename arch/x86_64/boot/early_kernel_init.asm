;Copyright (C) 2018  Igor Muravyov <igor.muravyov.2015@gmail.com>
bits 64
extern start_kernel
extern boot_magic
extern boot_info
extern stack_top

global early_kernel_init
; kernel x64 port early initialization
section .text
early_kernel_init:
        ;LONG MODE STACK REINITIALIZATION
        mov rsp, stack_top
        mov rdi,  [boot_magic] ; EAX <-- boot magic (via multiboot spec)
        mov rsi,  [boot_info]  ; EBX <-- boot info
.start_kernel:
        call start_kernel
        jmp .start_kernel
