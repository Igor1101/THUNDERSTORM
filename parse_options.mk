# here choose one of output modes "USE_VESA", "USE_VGA"
ifeq ($(strip $(KERNEL_OUTPUT)),)
	KERNEL_OPTIONS += -D USE_VESA
else
	KERNEL_OPTIONS += -D $(KERNEL_OUTPUT)
endif
# (Y/N) use serial as output
ifeq ($(KERNEL_USE_SERIAL), Y)
	KERNEL_OPTIONS += -D USE_SERIAL
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

