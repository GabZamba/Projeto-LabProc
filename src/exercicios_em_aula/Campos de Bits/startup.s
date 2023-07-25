.text
.global start

/*
 * Ponto de entrada do programa.
 */
start:
   /*
    * Configura modo do processador e pilha
    */
   mov r0, #0b10011              // modo SVR
   msr cpsr, r0
   ldr r13, =inicio_stack

   /*
    * Zera área bss.
    */
   mov r0, #0
   ldr r1, =inicio_bss
   ldr r2, =fim_bss
loop:
   cmp r1, r2
   bge cont
   str r0, [r1], #4
   b loop;
cont:
   bl main

// se main() retornar...
stop:
   b stop
