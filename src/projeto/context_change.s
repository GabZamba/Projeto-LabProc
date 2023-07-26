
.text

/*
 * Retorna a identificação do thread atual.
 */
 .global getid
getid:
    ldr r0, =tid
    ldr r0, [r0]
    movs pc, lr

/*
 * Troca de contextos.
 * Aqui é onde a mágica acontece
 */
.global thread_switch
thread_switch:
    /*
    * Salva o contexto do usuário no Task Control Block
    */
    push {r0}
    ldr r0, =curr_tcb
    ldr r0, [r0]

    // registradores r1-r14 do usuário
    stmib r0, {r1-r14}^          

    // salva endereço de retorno (lr)
    str lr, [r0, #60]

    // copia o spsr do usuário em r1 e salva no tcb
    mrs r1, spsr
    str r1, [r0, #64]

    // finalmente, salva o r0 original no tcb
    pop {r1}
    str r1, [r0]

    // escala o próximo processo e em seguida troca de contexto
    bl schedule

.global context_change
context_change:
    /*
    * Retorna no conexto de outro thread
    */
    // carrega a Task Control Block
    ldr r0, =curr_tcb
    ldr r0, [r0]

    // restaura spsr do usuário.
    ldr r1, [r0, #64]
    msr spsr, r1

    // Restaura r1-r14 originais, lr de retorno e finalmente r0
    ldmib r0, {r1-r14}^
    ldr lr, [r0, #60]
    ldr r0, [r0]

    // retorna para o thread, mudando o modo 
    movs pc, lr         // retorno SWI
    @ subs pc, lr, #4     // retorno IRQ