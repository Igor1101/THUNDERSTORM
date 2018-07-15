#Copyright (C) 2018  Igor Muravyov <igor.muravyov.2015@gmail.com>

# here choose one of output modes "USE_VESA", "USE_VGA", "NO_VIDEOMODE":
# USE_VESA - default
# USE_VGA - for legacy systems and debbuging purposes on real hardware 
# note, that kernel is not supported with legacy videocards, such as cirrus,
# use vga console instead.
# NO_VIDEOMODE - for embedded systems
KERNEL_OUTPUT=

# (Y/N) use serial as output
KERNEL_USE_SERIAL=Y
# choose one of "RELEASE", "DEBUG" 
# debug mode:
# -early output enabled (still experimental)
# -VGA mode output slowdown to make it more readable on real hardware
KERNEL_BUILD_MODE=DEBUG

