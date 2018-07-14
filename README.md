![THUNDERSTORM_LOGO](boot/TH.jpg)
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
GNU make,
nasm,
gcc(version >=7), 
binutils, 
coreutils,
Unix shell(sh),
qemu-system-x86_64(optional),
OVMF(optional), 
grub-mkrescue,
xorriso
#### compilation have been verified under:
Arch linux,
FreeBSD 11
#### Running it on qemu!
please consult make tasks:
```
make help
```
Running os with different BIOS(for example OVMF.fd, which is UEFI/BIOS):
```
make run BIOS=$(find /usr/share -name OVMF_CODE.fd)
```
Note, that THUNDERSTORM aims to be BIOS independent, 
but it is much better in qemu to run it with UEFI BIOS, since
it is better supported.

#### TODO this month (in case someone want`s to contribute):
- [x] add serial driver
- [ ] improve exceptions handling
- [ ] TSS initialization
- [ ] add multiboot 1.0 support to use more lightweight bootloader
- [ ] port multitasking from [THUNDERSTORM-OLD](http://github.com/Igor1101/THUNDERSTORM-OLD)
