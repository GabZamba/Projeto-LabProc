.include "evlt7t.inc"

.set SVR_MODE, 0b10011

/**
 * ARM Interruption Vector
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
swi_addr:   .word treat_swi
irq_addr:   .word treat_irq

.text
reset:
    // Sets processor on SVR mode
    mov r0, #SVR_MODE
    msr cpsr, r0
    ldr sp, =stack_svr

    // Nulls .bss segment
    mov r0, #0
    ldr r1, =bss_start
    ldr r2, =bss_end
loop_zera:
    cmp r1, r2
    bge start
    str r0, [r1], #4
    b loop_zera
start:
    bl initializeScheduler
    bl gpio_init
    b context_change

.global stop
stop:
    b stop

treat_swi:
    cmp r0, #1          // yield: schedules next thread
    beq thread_switch

    cmp r0, #2          // getpid: returns current threadId
    beq getid

    cmp r0, #3          // thread_exit: destroys current thread
    beq close_thread

    movs pc, lr         // returns from interruption

treat_irq:
    push {r0-r2}
    ldr r0, =INTPND
    ldr r1, [r0]

    // treat timer 0 interruption
    ands r2, r1, #(1 << 10)         // checks interruption 10 (timer 0)
    beq ack                         // if not pending, acknowledge all

    str r2, [r0]                    // acknowledge timer 0 interruption
    pop {r0-r2}
    b thread_switch

ack:
    str r1, [r0]                    // acknowledge all pending interruptions
    pop {r0-r2}
    subs pc, lr, #4                 // returns from IRQ
