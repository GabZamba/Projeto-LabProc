.section .reset, "ax"
.org 0
// vec
b _reset
b _undefined
b _swi
b _abort1
b _abort2
nop
b _irq
b _fiq

_reset:
    b init

_undefined:
    mov r0, #0x11
    b _undefined

_swi:
    mov r0, #0x22
    b _swi

_abort1:
    mov r0, #0x33
    b _abort1

_abort2:
    mov r0, #0x44
    b _abort2

_irq:
    b trata_irq

_fiq:
    mov r0, #0x66
    b _fiq