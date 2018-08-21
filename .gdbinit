set disassembly-flavor intel
target remote localhost:1234
file boot/THkernel
#break _start
break start_kernel
tui enable
layout src
focus src
layout reg
continue
