; Copyright (C) 2018  Igor Muravyov <igor.muravyov.2015@gmail.com>
; die before videomode initialized
extern kputstr_32
global die
RED equ 0x4f
; output works only if VGA mode enabled and rdi<=2^32
; note, that we are running 32-bit code ( kputstr_32 ) in long mode,
; may cause double fault.
die: 
;edi <--- reason
        mov  si,   RED
        call kputstr_32
.die:   hlt
        jmp .die
section .rodata
section .text
