# here choose one of output modes "CONFIG_USE_VESA", "CONFIG_USE_VGA"
ifeq ($(strip $(KERNEL_OUTPUT)),)
	KERNEL_OPTIONS += -D CONFIG_USE_VESA
else
	KERNEL_OPTIONS += -D $(KERNEL_OUTPUT)
endif
# (Y/N) use serial as output
ifeq ($(KERNEL_USE_SERIAL), Y)
	KERNEL_OPTIONS += -D CONFIG_USE_SERIAL
endif
	
# choose one of "RELEASE", "DEBUG" 
# debug mode:
# -early output enabled (still experimental)
# -VGA mode output slowdown to make it more readable
ifeq ($(strip $(KERNEL_BUILD_MODE)),)
	KERNEL_OPTIONS += -D RELEASE
else
	KERNEL_OPTIONS += -D $(KERNEL_BUILD_MODE)
endif

# VIDEO CONFIGURATION
KERNEL_OPTIONS += -D CONFIG_FB_WIDTH=$(CONFIG_FB_WIDTH)
KERNEL_OPTIONS += -D CONFIG_FB_HEIGHT=$(CONFIG_FB_HEIGHT)
KERNEL_OPTIONS += -D CONFIG_FB_BPP=$(CONFIG_FB_BPP)

# Add ARCH
KERNEL_OPTIONS += -D __$(strip $(ARCH))__ \
		  -D __KERNEL__ \
		  -D __THUNDERSTORM__ \
		  -U __linux__ \
		  -U __LINUX
