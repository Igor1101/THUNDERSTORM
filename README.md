![THUNDERSTORM_LOGO](TH.jpg)
**Is an embedded UNIX-like system**, which aims 
to be as portable as possible and use modern hardware features when possible.
Have similar to UNIX sources tree:
#### System programs sources:
```
src/ 
```
#### Portable kernel sources:
```
kernel/
```
#### HW dependent kernel sources:
```
arch/
```
#### Kernel libc:
```
libc/
```
....

##### Dependencies:
nasm gcc binutils qemu-system-x86_64 OVMF(optional) grub-mkrescue
#### Running it on qemu!
```
make
make run 
```
Running os with different BIOS(for example OVMF.fd, which is UEFI/BIOS):
```
make run BIOS=path_to_BIOS
```
