[bits    32]
[extern main]

section .text
global start
start:
    call main
    hlt
