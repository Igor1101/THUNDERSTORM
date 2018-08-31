
global kmemcpy_ptr
kmemcpy_ptr:  
  push rcx 
  push rsi 
  push rdi 
  push rdi
;rdi <- dest
;rsi <- src
;rdx <- count
  mov rcx, rdx
  rep movsq
  pop rax  ; return value 
  pop rdi 
  pop rsi 
  pop rcx 
  ret
