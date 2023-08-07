
#include <stdint.h>
#include <string.h> // Incluir o cabeçalho para a função memset
#include "gpio_utils.h"
#include "buffer.h"
#include "threads.h"

Scheduler scheduler = {};

// Pontos de entrada dos threads
void *main(void *);

void stop(void);           // rótulo stop no interrupt.s

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

void initializeScheduler(void)
{
    for (int i = 0; i < SCHEDULER_SIZE; i++)
    {
        initBuffer(&(scheduler.buffers[i]));
    }

    uint8_t id = 0;
    uint8_t *threadId = &id;

    thread_create(threadId, NULL, main, NULL);

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
    disableTimer0();

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

    setDisplayNumber(tid);
    setLedsValue(curr_tcb->priority);

    enableTimer0();
    resetTimer0Count();

    return;
}
