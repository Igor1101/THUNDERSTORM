global load_idt
; rdi: first IDT entry address
load_idt:
  mov   rax, rdi
  lidt [rax]
  ret
