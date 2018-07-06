/*
 * Copyright (C) 2018  Igor Muravyov <igor.muravyov.2015@gmail.com>
 */
#ifndef KOPT_H
#define KOPT_H

/* this file verifies predefined options for kernel */
#if !defined(USE_VGA) && !defined(USE_VESA)
#error "DEFINE ONE OF VIDEO MODES"
#endif				/* VIDEOMODE */

#endif				/* KOPT_H */
