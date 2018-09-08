; Copyright (C) 2018  Igor Muravyov <igor.muravyov.2015@gmail.com>
; early x86 kernel output(works only if VGA supported)
VGAROWS equ 25
VGA equ 0xb8000
bits 32
;
global kputstr_32
;print str to row
kputstr_32:
; edi <- pointer to str;
; esi <- colormode;
%ifdef CONFIG_USE_VGA
section .data
.row:   dq 0;for row info
.cons0: dq 0;
section .text
   push   ecx
   push   edx
; string row manipulation
    mov   edx,    [.row] ; row
    cmp   edx,    VGAROWS
    cmovnle edx,  [.cons0]; real crutch!!! start put 
    mov   ebx,    edx    ; ebx <- row
    add   edx,    1
    mov [.row], edx
; put str
    mov edx, edi
    mov ax,  si          ; color
    mov ah,  al
    mov ecx, 0           ; column init(always put to the beginning)
.kputstr_32_jmp:
    mov al, [edx]        ; ax <- char 
    call kputchar_32     ;print al
    add ecx, 1           ; ecx <- column
    add edx, 1
    cmp al,  0 ; while chr is not '\0'
    jne .kputstr_32_jmp
   pop edx
   pop ecx
%endif ; CONFIG_USE_VGA
   ret

; print char to VGA + 2*(row * 80 + col)
kputchar_32:; print value; shouldn`t be  global
; ax <- char
; ebx <- row
; ecx <- col
   push ebx
    imul ebx, 80
    add ebx, ecx
    shl ebx, 1
    mov word [VGA + ebx], ax
   pop ebx
   ret
