; long mode initialization
VGA equ 0xb8000
GREEN equ 0x2
RED equ 0x4f
VGAROWS equ 25
global _start
section .text
bits 32
_start:
    mov esp,  stk_top ; creating stack:
    mov cl,   GREEN ;<- <cl> color info;
section .rodata
thinfo: db "THUNDERSTORM 0.0 Embedded system x86_64 port layer",0
chk_multiboot: db "multiboot checked!", 0
chk_cpuid: db "cpuid checked!",0
section .text
    call check_multiboot ; <- verify are we multibooted? (uses eax );
    mov eax,  chk_multiboot
    call kputstr_to
    mov eax,  thinfo
    call kputstr_to
    call check_cpuid
    mov eax,  chk_cpuid
    call kputstr_to
    call check_long_mode
    hlt

check_multiboot:
    cmp eax, 0x36d76289; magic bootloader value
    jne .no_multiboot
    ret

.no_multiboot:
    mov eax, .nomultiboot_err
    jmp error
section .rodata
.nomultiboot_err: db "OOPs: not loaded as multiboot!!!" 
section .text

check_cpuid:
    push ecx
    ; Check if CPUID is supported by attempting to flip the ID bit (bit 21)
    ; in the FLAGS register. If we can flip it, CPUID is available.

    ; Copy FLAGS in to EAX via stack
    pushfd
    pop eax

    ; Copy to ECX as well for comparing later on
    mov ecx, eax

    ; Flip the ID bit
    xor eax, 1 << 21

    ; Copy EAX to FLAGS via the stack
    push eax
    popfd

    ; Copy FLAGS back to EAX (with the flipped bit if CPUID is supported)
    pushfd
    pop eax

    ; Restore FLAGS from the old version stored in ECX (i.e. flipping the
    ; ID bit back if it was ever flipped).
    push ecx
    popfd

    ; Compare EAX and ECX. If they are equal then that means the bit
    ; wasn't flipped, and CPUID isn't supported.
    cmp eax, ecx
    je .no_cpuid
    pop ecx
    ret
.no_cpuid:
section .rodata
   chk_cpuid_failed: db "CPUID is not supported ",0
section .text
    mov eax, chk_cpuid_failed
    jmp error

check_long_mode:
    ; test if extended processor info in available
    mov eax, 0x80000000    ; implicit argument for cpuid
    cpuid                  ; get highest supported argument
    cmp eax, 0x80000001    ; it needs to be at least 0x80000001
    jb .no_long_mode       ; if it's less, the CPU is too old for long mode

    ; use extended info to test if long mode is available
    mov eax, 0x80000001    ; argument for extended processor info
    cpuid                  ; returns various feature bits in ecx and edx
    test edx, 1 << 29      ; test if the LM-bit is set in the D-register
    jz .no_long_mode       ; If it's not set, there is no long mode
    ret
.no_long_mode:
section .rodata
   chk_long_failed: db "this CPU is not supported by this OS",0
section .text
    mov eax, chk_long_failed
    jmp error
error:
    mov cl, RED
    call kputstr_to
    hlt

;print str to row
kputstr_to:
; eax <- pointer to str;
; cl <- colormode;
section .rodata
.row:   dq 0;for row info
section .text
   push eax
   push ecx
   push edx
   push ebx
; string row manipulation
    mov edx, [.row] ; row
    cmp edx, VGAROWS
    jnae .notfilled
    mov edx, 1 ; real crutch!!! start put 
               ; into beginning of console( just after COPYRIGHT)
.notfilled:
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
kputchar_to:; print value
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
 ; memory, reserved for STACK:
section .bss
stk_bottom:
  resb 1024
stk_top:
   
