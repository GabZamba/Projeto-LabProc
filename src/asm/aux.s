/*
 * Função utilitária delay.
 */
.global delay
delay:
   subs r0, r0, #1
   bne delay
   mov pc, lr
