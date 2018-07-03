#Copyright (C) 2018  Igor Muravyov <igor.muravyov.2015@gmail.com>

# here choose one of output modes "USE_VESA", "USE_VGA"
KERNEL_OUTPUT=USE_VGA

# (Y/N) use serial as output
KERNEL_USE_SERIAL=Y
# choose one of "RELEASE", "DEBUG" 
# debug mode:
# -early output enabled (still experimental)
# -VGA mode output slowdown to make it more readable
KERNEL_BUILD_MODE=RELEASE

