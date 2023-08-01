
#include <stdint.h>
#include <string.h> // Incluir o cabeçalho para a função memset
#include "gpio_utils.h"
#include "buffer.h"
#include "threads.h"

Scheduler scheduler = {};

// Pontos de entrada dos threads
void func1(void *);
void func2(void *);
void main(void *);

void stop(void);           // rótulo stop no interrupt.s
void context_change(void); // rótulo context_change no interrupt.s

void destroy_thread(void); // declaracao da funcao final de cada thread

volatile int tid; // identificador do thread atual (0 ou 1)
tcb_t *curr_tcb;  // tcb do thread atual
Buffer *curr_buffer;

void getNextThread(Scheduler *scheduler, Buffer **nextBuffer, tcb_t **nextThread)
{
    for (int i = 0; i < SCHEDULER_SIZE; i++)
    {
        *nextBuffer = &(scheduler->buffers[i]);
        if ((*nextBuffer)->isEmpty)
            continue;
        *nextThread = &((*nextBuffer)->queue[(*nextBuffer)->start]);
        return;
    }
    // se todos estiverem vazios, retorna ao reset (pc e lr são 0)
    *nextBuffer = &(scheduler->buffers[0]);
    *nextThread = &((*nextBuffer)->queue[(*nextBuffer)->start]);
    return;
}
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

void initializeScheduler(void)
{
    for (int i = 0; i < SCHEDULER_SIZE; i++)
    {
        initBuffer(&(scheduler.buffers[i]));
    }
    // initBuffer(&buffer);

    uint8_t id = 0;
    uint8_t *threadId = &id;

    thread_create(threadId, NULL, main, NULL);
    // thread_create(threadId, NULL, func1, NULL);
    // thread_create(threadId, NULL, func2, NULL);

    getNextThread(&scheduler, &curr_buffer, &curr_tcb);

    tid = curr_tcb->tid;

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

    if (!dequeue(curr_buffer, &prev_tcb))
        stop();

    if (enqueueAgain)
    {
        if (!enqueue(curr_buffer, &prev_tcb))
            stop();
    }

    getNextThread(&scheduler, &curr_buffer, &curr_tcb);

    // gets the next executing thread
    tid = curr_tcb->tid;

    // blinkNumber(tid); // blinks the new thread id

    enableTimer();
    resetTimer();

    return;
}
