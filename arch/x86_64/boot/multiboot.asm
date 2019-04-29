;/*
; _____ _   _ _   _ _   _ ____  _____ ____  ____ _____ ___  ____  __  __        __
;|_   _| | | | | | | \ | |  _ \| ____|  _ \/ ___|_   _/ _ \|  _ \|  \/  |  __  /  \_  _
;  | | | |_| | | | |  \| | | | |  _| | |_) \___ \ | || | | | |_) | |\/| |_/  \/     \/ \
;  | | |  _  | |_| | |\  | |_| | |___|  _ < ___) || || |_| |  _ <| |  | |\_ ___________/
;  |_| |_| |_|\___/|_| \_|____/|_____|_| \_\____/ |_| \___/|_| \_\_|  |_|   /
;                                                                          /\_
;                                                                          \  \
;__________________________________________________________________________/___________
; 
;                     Embedded operating system
;    Copyright (C) 2018-2019  Igor Muravyov <igor.muravyov.2015@gmail.com>
;
;    This program is free software: you can redistribute it and/or modify
;    it under the terms of the GNU General Public License as published by
;    the Free Software Foundation, either version 3 of the License, or
;    (at your option) any later version.
;
;    This program is distributed in the hope that it will be useful,
;    but WITHOUT ANY WARRANTY; without even the implied warranty of
;    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;    GNU General Public License for more details.
;
;    You should have received a copy of the GNU General Public License
;    along with this program.  If not, see <http://www.gnu.org/licenses/>.
;
;*/

; MULTIBOOT2 HEADER
MAGIC equ 0xe85250d6
ARCH  equ 0 ;/* i386 */
section .multiboot
multiboot_start:
  dd MAGIC
  dd ARCH
  dd multiboot_end - multiboot_start
  dd 0x100000000 - (MAGIC + ARCH + (multiboot_end - multiboot_start))
address_tag_start:

%ifdef CONFIG_USE_VESA
framebuffer_tag:
  dw 5 ; 
  dw 1 ; optional
  dd .end - framebuffer_tag ; size
  dd CONFIG_FB_WIDTH  ;width
  dd CONFIG_FB_HEIGHT ;heiht
  dd CONFIG_FB_BPP ;bpp
.end:
%endif
;module_alignment_tag:
;  dw 6
;  dw 1
tags_end:
; terminate tags:
  dw 0
  dd 8
multiboot_end:
