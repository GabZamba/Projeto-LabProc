.set IOPDATA, 0x3ff5008
.set TDATA1, 0x03ff6008 // registrador de recarda do temp1

.set TEMPO, 49999999    // valor de recarga para 1s em 50 MHz

.global chaves
chaves:
    ldr r4, =IOPDATA
    ldr r5, [r4]

    // carrega o valor das chaves
    mov r1, #15
    and r1, r1, r5 

    // escreve as chaves nos leds
    mov r2, r1, lsl #4              // desloca chaves para os leds
    bic r5, r5, #(0b1111 << 4)      // reinicia leds
    orr r5, r5, r2                  // liga os bits correspondentes às chaves

    str r5, [r4]                    // acende leds

    // chave 4 incrementa (1) ou decrementa (0)
    // chave 3 não faz nada
    // chave 2 torna mais devagar (1) ou nada (0)
    // chave 1 torna mais rapido (1) ou nada (0)
    
    // carrega o valor default de tempo (1s)
    ldr r5, =TEMPO

chave4:
    tst r1, #0b1 // verifica se chave 4 é 1, se for
    beq decrementar // senão
        add r0, r0, #1
chave2:
    tst r1, #0b100 // verifica chave 2 (4x mais rapido)
    beq acelerar
chave1:
    tst r1, #0b1000 // verifica chave 1 (4x mais devagar)
    beq freiar
salvar_chaves:
    ldr r4, =TDATA1
    str r5, [r4]

    bx lr

decrementar:
    sub r0, r0, #1
    b chave2
acelerar:
    mov r5, r5, lsl #2
    b chave1
freiar:
    mov r5, r5, lsr #2
    b salvar_chaves
