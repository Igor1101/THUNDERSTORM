; early x86 kernel output(works only if VGA supported)
VGAROWS equ 25
VGA equ 0xb8000
bits 32
;
global kputstr_to
;print str to row
kputstr_to:
; eax <- pointer to str;
; cl <- colormode;
section .data
.row:   dq 0;for row info
.cons0: dq 0;
section .text
   push eax
   push ecx
   push edx
   push ebx
; string row manipulation
    mov edx, [.row] ; row
    cmp edx, VGAROWS
    cmovnle edx,  [.cons0]; real crutch!!! start put 
    add edx, 1
    mov ebx, edx
    mov [.row], edx
; put str
    mov edx, eax
    mov ah, cl; color
    mov ecx, 0   ; column
.kputstr_to_jmp:
    mov al, [edx]; char
    call kputchar_to ;print al
    add ecx, 1
    add edx, 1
    cmp al, 0 ; while str is not empty
    jne .kputstr_to_jmp
   pop ebx
   pop edx
   pop ecx
   pop eax
   ret

; print char to VGA + 2*(row * 80 + col)
kputchar_to:; print value; shouldn`t be  global
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
