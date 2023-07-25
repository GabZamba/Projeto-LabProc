.set MODO_SVR, 0b10011
.section .reset, "ax"
.org 0
// vetor interrupções
b _reset
b _undefined
b _swi
b _abort1
b _abort2
nop
b _irq
b _fiq

_reset:
    ldr r0, =_main
    mov pc, r0

_undefined:
    b check_undefined

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
    mov r0, #0x55
    b _irq

_fiq:
    mov r0, #0x66
    b _fiq

check_undefined:
    @ mov sp, #0x41000000
    
    push {r0-r2}        // salva o valor atual dos registradores que são utilizados como variáveis
    ldr r0, =formato_instrucao
    ldr r0, [r0]        // R0 é o valor da instrução criada
    ldr r1, [lr, #-4]   // R1 é o valor da instrução que gerou o undefined (end anterior ao link register)

    // verifica se a instrução que gerou o undefined é a criada (compara zerando)
    subs r0, r1, r0     // se for a mesma instrução, zera todos os bits menos os 4 últimos
    ands r1, r0, #-16   // zera os 4 últimos bits -> 0 se for a mesma instrução (flag Z ativa)
    beq nova_instrucao

// loop infinito caso a instrução que gerou o undefined não seja a criada
kernel_panic: 
    mov r0, #0
    b kernel_panic

nova_instrucao:
    ldr r2, =inverter_regX
    mov pc, r2


.data
formato_instrucao:
    .word 0xEE000900


.text
_main:
    mov r0, #MODO_SVR
    msr cpsr, r0
    mov sp, #0x41000000

    mov r0, #0  // -1
    mov r1, #1  // -2
    mov r2, #2  // -3
    mov r3, #3  // ...
    mov r4, #4
    mov r5, #5
    mov r6, #6
    mov r7, #7
    mov r8, #8
    mov r9, #9
    mov r10, #10
    mov r11, #11
    mov r12, #12    // -13

    cdp p9, 0, c0, c0, c0, 0
    cdp p9, 0, c0, c0, c1, 0
    cdp p9, 0, c0, c0, c2, 0
    cdp p9, 0, c0, c0, c3, 0
    cdp p9, 0, c0, c0, c4, 0
    cdp p9, 0, c0, c0, c5, 0
    cdp p9, 0, c0, c0, c6, 0
    cdp p9, 0, c0, c0, c7, 0
    cdp p9, 0, c0, c0, c8, 0
    cdp p9, 0, c0, c0, c9, 0
    cdp p9, 0, c0, c0, c10, 0
    cdp p9, 0, c0, c0, c11, 0
    cdp p9, 0, c0, c0, c12, 0
    cdp p9, 0, c0, c0, c13, 0
    cdp p9, 0, c0, c0, c14, 0
    cdp p9, 0, c0, c0, c15, 0
    
loop:
    mov r0, r0
    b loop

inverter_regX:
    // desvia se o valor do registrador for proibido
    cmp r0, #0
    beq inst_r0
    cmp r0, #12
    bgt invalid_reg

    mov r0, r0, lsl #12  // salva em r0 o bit correspondente ao registrador a ser invertido

    // altera o valor da instrução em pushX, acendendo o bit correspondente ao valor do r0
    ldr r2, =pushX
    ldr r1, [r2]
    bic r1, r1, #(0xf << 12)      // limpa os 4 bits do registrados
    orr r1, r1, r0      // acende apenas os bits do reg correspondente ao r0
    str r1, [r2]        // salva a instrução alterada

    // altera o valor da instrução em popX, acendendo o bit correspondente ao valor do r0
    ldr r2, =popX
    ldr r1, [r2]
    bic r1, r1, #(0xf << 12)      // limpa os 16 bits menos significativos
    orr r1, r1, r0      // acende apenas o bit do regcorrespondente ao r0
    str r1, [r2]        // salva a instrução alterada

    pop {r0-r2}         // restaura o valor de todos os registradores

    push {r0}
    push {r1}           // salva o valor de r1 para ser recuperado

pushX:
    push {r12}          // instrução alterada dinamicamente, salva na pilha o valor a ser invertido
    
    pop {r0}            // carrega para r0 o valor a ser invertido
    mov r1, #-1         // operação de inverter
    eor r0, r0, r1

    pop {r1}            // restaura o valor de r1

    push {r0}           // salva o valor invertido na pilha

popX:
    pop {r12}           // instrução alterada dinamicamente, recupera o valor invertido para o registrador correspondente
    pop {r0}
end:
    movs pc, lr

inst_r0:
    pop {r0-r2}         // restaura R0 a R2
    push {r1}           // salva R1
    mov r1, #-1         // operação de inverter
    eor r0, r0, r1
    pop {r1}            // restaura R1
    b end

invalid_reg:
    pop {r0-r2}         // restaura R0 a R2
    b end