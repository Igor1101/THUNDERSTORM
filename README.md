![THUNDERSTORM_LOGO](TH.jpg)
**Is an embedded UNIX-like system**, which aims 
to be as portable as possible and use modern hardware features when possible.
Have similar to UNIX sources tree:
```
src/ # System programs sources
bin/ #system programs binaries
kernel/ # Portable kernel sources
libc/ # Kernel libc
arch/ # HW dependent kernel sources
arch/$(ARCH)/boot # HW dependent initialization
arch/$(ARCH)/lld/ # low level built-in drivers
arch/$(ARCH)/include/asm # headers, those provide machine-independent symbols
# we can use them in high level code
arch/$(ARCH)/include/$(ARCH) # specific to machine symbols/namespaces
```
....

##### Dependencies:
nasm gcc binutils qemu-system-x86_64 OVMF(optional) grub-mkrescue
#### Running it on qemu!
```
make # elf image of kernel is placed to "boot/THkernel", "boot/" is placed to cd image "os.iso" with grub bootloader installed
make run
```
Running os with different BIOS(for example OVMF.fd, which is UEFI/BIOS):
```
make run BIOS=$(find /usr/share -name OVMF_CODE.fd)
```
Note, that THUNDERSTORM aims to be BIOS independent, 
but it is much better in qemu to run it with UEFI BIOS, since
it is better supported.

#### TODO this month (in case someone want`s to contribute):
- [ ] improve exceptions handling
- [ ] TSS initialization
- [ ] add multiboot 1.0 support to use more lightweight bootloader
- [ ] port multitasking from @github.com/Igor1101/THUNDERSTORM-OLD
