.set INTPND, 0x03ff4004 // interrupções pendentes em 1
.set IOPMOD, 0x3ff5000
.set IOPDATA, 0x3ff5008

.set MODO_SVR, 0b10011

.text

.global init
// inicialização
init:
    mov r0, #MODO_SVR
    msr cpsr, r0
    mov r13, #0x8000

    mov r0, #0

    bl init_gpio
    bl init_timer1                  // inicializa o timer1
    bl hexa7seg                     // liga o display em 0
    bl chaves

loop:
    mov r0, r0
    b loop


.global trata_irq
// chamado pelo vetor de interrupções
trata_irq:
    ldr r4, =INTPND
    ldr r5, [r4]

    tst r5, #(1 << 11)              // verifica interrupção 11 (timer 1)
    beq ack                         // se não houver, reconhece todas

    // tratamento da interrupção do timer 1
    push {r4-r5,lr}                 // salva os valores de r4 e r5
    bl chaves
    bl hexa7seg
    pop {r4-r5,lr}                  // recupera os valores de r4 a r5

ack:
    str r5, [r4]                    // reconhece todas as interrupções
    subs pc, lr, #4                 // retorna IRQ


init_gpio:
    // define leds como saída
    ldr r4, =IOPMOD
    ldr r5, [r4]
    orr r5, r5, #(0b1111 << 4)      // bic entrada, orr saída
    str r5, [r4]

    // define o display como saída
    ldr r4, =IOPMOD
    ldr r5, [r4]
    orr r5, r5, #(0b1111111 << 10)  // bic entrada, orr saída
    str r5, [r4]

    // define chaves como entrada
    ldr r4, =IOPMOD
    ldr r5, [r4]
    bic r5, r5, #0b1111              // bic entrada, orr saída
    str r5, [r4]

    bx lr


hexa7seg:
    // carrega em r1 os 4 bits menos significativos de r0
    mov r1, #15 
    and r1, r1, r0

    // salva em r3 os 7 bits correspondentes ao hexa em r1 (endereço [tabela + r1])
    ldr r2, =tabela
    ldrb r3, [r2, r1]

    // desloca os 7 bits para a posição correta
    mov r3, r3, lsl #10

    // faz o display ligar
    ldr r4, =IOPDATA
    ldr r5, [r4]
    bic r5, r5, #(0b1111111 << 10)  // reseta os bits do display
    orr r5, r5, r3                  // bic seria apagar, orr acende
    str r5, [r4]

    bx lr

.data

// tabela que contém todos os valores de 0 a F para os 7 bits do display
tabela:
    .byte 0b1011111, 0b0000110, 0b0111011, 0b0101111, 0b1100110, 0b1101101, 0b1111101, 0b0000111, 0b1111111, 0b1101111, 0b1110111, 0b1111100, 0b1011001, 0b0111110, 0b1111001, 0b1110001
