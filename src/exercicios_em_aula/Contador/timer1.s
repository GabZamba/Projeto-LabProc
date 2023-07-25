.set INTMOD, 0x03ff4000 // 1 se IRQ, 0 se FIQ (rápida)
.set INTMSK, 0x03ff4008 // 1 se mascarado, 0 se habilitado
.set TMOD, 0x03ff6000   // configuração dos temporizadores
.set TDATA1, 0x03ff6008 // registrador de recarda do temp1

.set TEMPO, 49999999    // valor de recarga para 1s em 50 MHz

.text

.global init_timer1
init_timer1:
    // configura interrupção 11 (timer 1) como IRQ
    ldr r2, =INTMOD
    ldr r1, [r2]
    bic r1, r1, #(1 << 11)      // configura como IRQ
    str r1, [r2]

    // habilita a interrupção 11 (timer1) e 21 (global)
    ldr r2, =INTMSK
    ldr r1, [r2]
    bic r1, r1, #(1 << 11)     
    bic r1, r1, #(1 << 21)    
    str r1, [r2]

    // configura valor de recarga de 1s (período)
    ldr r2, =TDATA1
    ldr r1, =TEMPO
    str r1, [r2]

    // liga o timer 1
    ldr r2, =TMOD
    ldr r1, [r2]
    bic r1, r1, #(0b111 << 3)    // reinicia modo do timer 1
    orr r1, r1, #(0b001 << 3)    // habilita o timer
    str r1, [r2]

    bx lr                       // retorna da subrotina
