
#include <stdint.h>
#include <string.h> // Incluir o cabeçalho para a função memset
#include "gpio_utils.h"
#include "buffer.h"

Buffer buffer = {};

// Definidos pelo linker:
extern uint8_t stack_usr1[];
extern uint8_t stack_usr2[];
extern uint8_t stack_usr3[];
extern uint8_t stack_svr[];

// Pontos de entrada dos threads
int func1(void);
int func2(void);
int main(void);

void stop(void);           // rótulo stop no interrupt.s
void context_change(void); // rótulo context_change no interrupt.s

void destroy_thread(void); // declaracao da funcao final de cada thread

volatile int tid; // identificador do thread atual (0 ou 1)
volatile tcb_t curr_tcb_value;
volatile tcb_t *curr_tcb = &curr_tcb_value; // tcb do thread atual

/**
 * Threads definidos no sistema.
 */
tcb_t tcb[3] = {
    /*
     * Contexto da thread principal.
     */
    {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // r0-r12
        (uint32_t)stack_usr3,                  // sp
        (uint32_t)destroy_thread,              // lr inicial
        (uint32_t)main,                        // pc = lr = ponto de entrada
        0x10,                                  // valor do cpsr (modo svr)
        0,                                     // prioridade
        1                                      // thread id
    },
    /*
     * Contexto do primeiro thread (func1).
     */
    {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // r0-r12
        (uint32_t)stack_usr1,                  // sp
        (uint32_t)destroy_thread,              // lr inicial
        (uint32_t)func1,                       // pc = lr = ponto de entrada
        0x10,                                  // valor do cpsr (modo usuário)
        0,                                     // prioridade
        2                                      // thread id
    },
    /*
     * Contexto do segundo thread (func2).
     */
    {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // r0-r12
        (uint32_t)stack_usr2,                  // sp
        (uint32_t)destroy_thread,              // lr inicial
        (uint32_t)func2,                       // pc = lr = ponto de entrada
        0x10,                                  // valor do cpsr (modo usuário)
        0,                                     // prioridade
        3                                      // thread id
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
        "mov r0, #3 \n\t"
        "swi #0     \n\t");
}

void initializeScheduler(void)
{
    initBuffer(&buffer);

    enqueue(&buffer, &tcb[0]);
    enqueue(&buffer, &tcb[1]);
    enqueue(&buffer, &tcb[2]);

    tid = buffer.queue[buffer.start].tid;
    *curr_tcb = buffer.queue[buffer.start];

    return;
}

/**
 * Escalador:
 * Escolhe o próximo thread.
 */
void schedule(bool enqueueAgain)
{
    disableTimer();

    // move current thread to the end of the queue
    tcb_t prev_tcb;

    if (!dequeue(&buffer, &prev_tcb))
        stop();

    if (enqueueAgain)
    {
        if (!enqueue(&buffer, curr_tcb))
            stop();
    }

    // gets the next executing thread
    *curr_tcb = buffer.queue[buffer.start];
    tid = curr_tcb->tid;

    // blinkNumber(tid); // blinks the new thread id

    enableTimer();
    resetTimer();

    return;
}
