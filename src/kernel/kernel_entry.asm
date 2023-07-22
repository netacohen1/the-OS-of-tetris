[bits    32]
[extern main]

section .entry
global _start
_start:
    call main
    hlt
