
#include <stdint.h>
#include <string.h> // Incluir o cabeçalho para a função memset
#include "gpio_utils.h"
#include "buffer.h"

Buffer buffer = {};

// Definidos pelo linker:
extern uint8_t stack_usr1[];
extern uint8_t stack_usr2[];

// Pontos de entrada dos threads
int main(void);
int main2(void);
void context_change(void); // rótulo stop no interrupt.s
void stop(void);           // rótulo stop no interrupt.s
void destroy_thread(void); // declaracao da funcao final de cada thread

/**
 * Threads definidos no sistema.
 */
tcb_t tcb[2] = {
    /*
     * Contexto do primeiro thread (main).
     */
    {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // r0-r12
        (uint32_t)stack_usr1,                  // sp
        (uint32_t)destroy_thread,              // lr inicial
        (uint32_t)main,                        // pc = lr = ponto de entrada
        0x10,                                  // valor do cpsr (modo usuário)
        0,                                     // prioridade
        0                                      // thread id
        // TODO: retorna ao loop do stop no interrupt.s
    },
    /*
     * Contexto do segundo thread (main2).
     */
    {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // r0-r12
        (uint32_t)stack_usr2,                  // sp
        0,                                     // lr inicial
        (uint32_t)main2,                       // pc = lr = ponto de entrada
        0x10,                                  // valor do cpsr (modo usuário)
        0,                                     // prioridade
        0                                      // thread id
        // TODO: CUIDADO, COMO LR ESTÁ EM 0, AO RETORNAR DA FUNÇÃO MAIN2, ELE RETORNA AO _RESET
    }};

/**
 * Chama o kernel com swi, a função "yield" (r0 = 1).
 * Devolve o controle ao sistema executivo, que pode escalar outro thread.
 */
void __attribute__((naked)) yield(void)
{
    asm volatile(
        "push {lr}  \n\t"
        "mov r0, #1 \n\t"
        "swi #0     \n\t"
        "pop {pc}");
}

/**
 * Retorna o thread-id do thread atual.
 */
int __attribute__((naked)) getpid(void)
{
    asm volatile(
        "push {lr}  \n\t"
        "mov r0, #2 \n\t"
        "swi #0     \n\t"
        "pop {pc}");
}
/**
 * Destrói a thread atual, executando a próxima na fila.
 */
void __attribute__((naked)) destroy_thread(void)
{
    asm volatile(
        // "push {lr}  \n\t"
        "mov r0, #3 \n\t"
        "swi #0     \n\t"
        // "pop {pc}"
    );
}

volatile int tid;                   // identificador do thread atual (0 ou 1)
volatile tcb_t *curr_tcb = &tcb[0]; // tcb do thread atual

void initializeScheduler(void)
{
    initBuffer(&buffer);

    enqueue(&buffer, &tcb[0]);
    enqueue(&buffer, &tcb[1]);

    tid = buffer.queue[buffer.start].tid;
    *curr_tcb = buffer.queue[buffer.start];

    return;
}

/**
 * Escalador:
 * Escolhe o próximo thread.
 */
void schedule(void)
{
    disableTimer();

    // move current thread to the end of the queue
    tcb_t prev_tcb;

    if (!dequeue(&buffer, &prev_tcb))
        stop();

    if (!enqueue(&buffer, curr_tcb))
        stop();

    // gets the next executing thread
    *curr_tcb = buffer.queue[buffer.start];
    tid = curr_tcb->tid;

    blinkNumber(tid); // blinks the new thread id

    enableTimer();
    resetTimer();

    return;
}

/**
 * Escalador:
 * Escolhe o próximo thread.
 */
void schedule2(void)
{
    disableTimer();

    // move current thread to the end of the queue
    tcb_t prev_tcb;

    if (!dequeue(&buffer, &prev_tcb))
        stop();

    // gets the next executing thread
    *curr_tcb = buffer.queue[buffer.start];
    tid = curr_tcb->tid;

    blinkNumber(tid); // blinks the new thread id

    enableTimer();
    resetTimer();
}