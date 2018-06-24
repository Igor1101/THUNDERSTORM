/* this file verifies predefined options for kernel */
#if !defined(USE_VGA) && !defined(USE_VESA)
#error "DEFINE ONE OF VIDEO MODES"
#endif
