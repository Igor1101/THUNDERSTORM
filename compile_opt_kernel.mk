#Copyright (C) 2018  Igor Muravyov <igor.muravyov.2015@gmail.com>

# here choose one of output modes "CONFIG_USE_VESA", "CONFIG_USE_VGA", "CONFIG_NO_VIDEOMODE":
# CONFIG_USE_VESA - default
# CONFIG_USE_VGA - for legacy systems and debbuging purposes on real hardware 
# note, that kernel is not supported with legacy videocards, such as cirrus,
# use vga console instead.
# CONFIG_NO_VIDEOMODE - for embedded systems
KERNEL_OUTPUT=

# VIDEO CONFIGURATION
# TODO : fix 1366x768 mode
# 640x480 recommended
CONFIG_FB_WIDTH=640
CONFIG_FB_HEIGHT=480

# BITS PER PIXEL 
# (only 32 is guarateed to work on all laptops)
CONFIG_FB_BPP=32

# (Y/N) use serial as output
KERNEL_USE_SERIAL=Y
# choose one of "RELEASE", "DEBUG" 
# debug mode:
# -early output enabled (still experimental)
# -VGA mode output slowdown to make it more readable on real hardware
KERNEL_BUILD_MODE=DEBUG


