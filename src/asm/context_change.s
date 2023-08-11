.set SVR_MODE, 0b10011
.set IRQ_MODE, 0b10010
.text

/*
 * Return current thread id
 */
 .global getid
getid:
    ldr r0, =tid
    ldr r0, [r0]
    movs pc, lr

/*
 * Save current thread context
 */
.global thread_switch
thread_switch:
    push {r0}           // used as auxiliar

    // change flags if current mode is IRQ (because of the differences between SVR and IRQ return)
    mrs r0, cpsr
    and r0, r0, #0x1f   // applies 5 bit mask (get cpsr mode)
    cmp r0, #IRQ_MODE

    subeq lr, lr, #4    // corrects LR if current mode is IRQ

    // Loads current thread context from Task Control Block
    ldr r0, =curr_tcb
    ldr r0, [r0]

    stmib r0, {r1-r14}^     // r1-r14 user registers
    str lr, [r0, #60]       // return address (lr)

    mrs r1, spsr            // saves user spsr
    str r1, [r0, #64]

    pop {r1}                // saves original r0
    str r1, [r0]

    // schedules next thread and changes context
    mov r0, #1      // enqueueAgain = true
    moveq r1, #1    // wasPreempted = true
    movne r1, #0    // wasPreempted = false
    bl schedule
    b context_change

/*
 * Return on the context of the next thread
 */
.global context_change
context_change:
    // loads the thread information from Task Control Block
    ldr r0, =curr_tcb
    ldr r0, [r0]

    // restores user spsr
    ldr r1, [r0, #64]
    msr spsr, r1

    // restores original r1-r14, return lr and r0
    ldmib r0, {r1-r14}^
    ldr lr, [r0, #60]
    ldr r0, [r0]

    // returns to the new thread (lr fixed on thread_switch)
    movs pc, lr     // SVR mode return

/**
 * Destroys current thread
 */
.global close_thread
close_thread:
    mov r0, r1          // load to r0 the pointer to the value returned by the thread
    bl save_return_pointer
    mov r0, #0          // enqueueAgain = false (destroy)
    mov r1, #0          // wasPreempted = false
    bl schedule
    b context_change
