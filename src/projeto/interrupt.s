@ .set INTPND, 0x03ff4004 // interrupções pendentes em 1
.set INTPND, 0x40100000 // QEMU

.set MODO_SVR, 0b10011

/**
 * Vetor de interrupções do ARM
 */
.section .reset, "ax"
.org 0
vetor:
_reset:
    ldr pc, reset_addr
_undef:
    b panic
_swi:
    ldr pc, swi_addr
_abort1:
    b panic
_abort2:
    b panic
nop
_irq:
    ldr pc, irq_addr
_fiq:
    b panic

panic:
    b panic

reset_addr: .word reset
swi_addr:   .word trata_swi
irq_addr:   .word trata_irq

.text
/*
 * Ponto de entrada após reset.
 */
reset:
    // Configura sp do modo SVR:
    mov r0, #MODO_SVR
    msr cpsr, r0
    ldr sp, =stack_svr

    // Zera segmento .bss:
    mov r0, #0
    ldr r1, =inicio_bss
    ldr r2, =fim_bss
loop_zera:
    cmp r1, r2
    bge start
    str r0, [r1], #4
    b loop_zera

/*
 * Ponto de entrada: executa o primeiro thread (tid = 0).
 */
start:
    bl initializeScheduler
    bl gpio_init
    b context_change
.global stop
stop:
    b stop

/*
 * Ponto de entrada do kernel.
 * Identifica a função solicitada e trata.
 */
trata_swi:
    cmp r0, #1          // função yield: troca de thread
    beq thread_switch

    cmp r0, #2          // função getpid: retorna a identificação do thread atual
    beq getid

    cmp r0, #3          // função getpid: retorna a identificação do thread atual
    beq close_thread

    // outras funções do kernel vão aqui...
    movs pc, lr          // retorna da interrupção

trata_irq:
    push {r0-r2}
    ldr r0, =INTPND
    ldr r1, [r0]

    ands r2, r1, #(1 << 10)         // verifica interrupção 10 (timer 0)
    beq ack                         // se não houver, reconhece todas

    str r2, [r0]                    // reconhece interrupção do timer
    // tratamento da interrupção do timer 0
    pop {r0-r2}
    b thread_switch

ack:
    str r1, [r0]                    // reconhece todas as interrupções
    pop {r0-r2}
    subs pc, lr, #4                 // retorna do IRQ
