
#include <stdint.h>
#include <string.h> // Incluir o cabeçalho para a função memset
#include "gpio_utils.h"
#include "buffer.h"
#include "threads.h"

Buffer buffer = {};

// Pontos de entrada dos threads
void func1(void *);
void func2(void *);
void main(void *);

void stop(void);           // rótulo stop no interrupt.s
void context_change(void); // rótulo context_change no interrupt.s

void destroy_thread(void); // declaracao da funcao final de cada thread

volatile int tid; // identificador do thread atual (0 ou 1)
volatile tcb_t curr_tcb_value;
volatile tcb_t *curr_tcb = &curr_tcb_value; // tcb do thread atual

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

    uint8_t id = 0;
    uint8_t *threadId = &id;

    thread_create(threadId, NULL, main, NULL);
    thread_create(threadId, NULL, func1, NULL);
    thread_create(threadId, NULL, func2, NULL);

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
