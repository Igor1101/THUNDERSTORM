extern kputstr_to
extern error
extern warning
global die

die: ; reason: rdi
section .rodata
.str: db "KERNEL DEAD\n",0
section .text
mov rax, rdi
mov cl,  5
call kputstr_to
mov rax, .str
call kputstr_to
.die:  hlt
jmp .die
