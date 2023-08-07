
#include <stdint.h>
#include <string.h> // Incluir o cabeçalho para a função memset
#include "gpio_utils.h"
#include "buffer.h"
#include "threads.h"

Scheduler scheduler = {};

// Pontos de entrada dos threads
void *main(void *);

void stop(void); // rótulo stop no interrupt.s

volatile int tid; // identificador do thread atual (0 ou 1)
tcb_t *curr_tcb;  // tcb do thread atual
Buffer *curr_buffer;

uint8_t QuantumData[SCHEDULER_SIZE] = {10, 8, 6, 4};

void getNextThread(Scheduler *scheduler, Buffer **nextBuffer, tcb_t **nextThread)
{
    bool allEmpty = true;
    // verifica se todos os buffers estao vazios
    for (int i = 0; i < SCHEDULER_SIZE; i++)
    {
        if (!scheduler->buffers[i].isEmpty) // se algum não está vazio
        {
            allEmpty = false;
            break;
        }
    }

    // se todos estiverem vazios, retorna ao reset (pc e lr são 0)
    if (allEmpty)
    {
        *nextBuffer = &(scheduler->buffers[0]);
        *nextThread = &((*nextBuffer)->queue[(*nextBuffer)->start]);
        (*nextThread)->cpsr = 0x13;
        return;
    }

    // se algum não estiver vazio, tenta pegar a próxima thread,
    // só consegue se algum quantumCnt != 0
    for (int i = 0; i < SCHEDULER_SIZE; i++)
    {
        // itera pelo scheduler, selecionando o primeiro buffer que não está vazio
        *nextBuffer = &(scheduler->buffers[i]);
        // se estiver vazio ou já tiver sido percorrido, vai à próxima iteração
        if ((*nextBuffer)->isEmpty || (*nextBuffer)->quantumCnt == 0)
        {
            (*nextBuffer)->quantumCnt = 0;
            continue;
        }
        *nextThread = &((*nextBuffer)->queue[(*nextBuffer)->start]);
        (*nextBuffer)->quantumCnt -= 1;
        return;
    }

    // chega aqui caso todas tenham quantumCnt == 0
    // reinicia os quantumCnt
    for (int i = 0; i < SCHEDULER_SIZE; i++)
    {
        scheduler->buffers[i].quantumCnt = scheduler->buffers[i].quantumData;
    }

    // pega a proxima thread
    for (int i = 0; i < SCHEDULER_SIZE; i++)
    {
        // itera pelo scheduler, selecionando o primeiro buffer que não está vazio
        *nextBuffer = &(scheduler->buffers[i]);
        // se estiver vazio ou já tiver sido percorrido, vai à próxima iteração
        if ((*nextBuffer)->isEmpty || (*nextBuffer)->quantumCnt == 0)
        {
            (*nextBuffer)->quantumCnt = 0;
            continue;
        }
        *nextThread = &((*nextBuffer)->queue[(*nextBuffer)->start]);
        (*nextBuffer)->quantumCnt -= 1;
        return;
    }
}

void initializeScheduler(void)
{
    for (int i = 0; i < SCHEDULER_SIZE; i++)
    {
        initBuffer(&(scheduler.buffers[i]), QuantumData[i]);
    }

    uint8_t id = 0;
    uint8_t *threadId = &id;

    thread_create(threadId, 0, main, NULL);

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
