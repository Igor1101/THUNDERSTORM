; Copyright (C) 2018-2019  Igor Muravyov <igor.muravyov.2015@gmail.com>
; die before panic initialized
global die
die: 
;edi <--- reason
.die:   hlt
        jmp .die
