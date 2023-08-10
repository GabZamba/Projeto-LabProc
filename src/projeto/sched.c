
#include <stdint.h>
#include <string.h> // Incluir o cabeçalho para a função memset
#include "gpio_utils.h"
#include "buffer.h"
#include "threads.h"

Scheduler scheduler = {};

// Pontos de entrada dos threads
extern void *main(void *);

void stop(void); // rótulo stop no interrupt.s

volatile int tid; // identificador do thread atual
tcb_t *curr_tcb;  // tcb do thread atual
Buffer *curr_buffer;

volatile uint64_t resetPriorityCount = 0;
volatile uint64_t resetPriorityData = (uint64_t)30 * ONE_SECOND;

uint8_t quantumData[SCHEDULER_SIZE] = {1, 2, 3, 4};

/**
 * Obtém a próxima thread a ser executada
 *
 * @param scheduler Ponteiro para o scheduler principal
 * @param nextBuffer Ponteiro para o próximo Buffer (será definido pela função)
 * @param nextThread Ponteiro para a próxima thread a ser executada (será definido pela função)
 */
void getNextThread(Scheduler *scheduler, Buffer **nextBuffer, tcb_t **nextThread)
{
    bool allBuffersEmpty = true;

    Buffer *buffers = &scheduler->buffers[0];

    // verifica se todos os buffers estao vazios ou já tiveram seu quantum esgotado
    for (int i = 0; i < SCHEDULER_SIZE; i++)
    {
        // verifica se há buffers não-vazios
        if (!buffers[i].isEmpty)
            allBuffersEmpty = false;
    }

    // se todos estiverem vazios, retorna ao reset (pc e lr são 0)
    if (allBuffersEmpty)
    {
        *nextBuffer = &buffers[0];
        *nextThread = &((*nextBuffer)->queue[(*nextBuffer)->start]);
        (*nextThread)->cpsr = 0x13;
        return;
    }

    // pega a proxima thread
    for (int i = 0; i < SCHEDULER_SIZE; i++)
    {
        // itera pelo scheduler, selecionando o primeiro buffer que não está vazio
        *nextBuffer = &buffers[i];

        // se estiver vazio, vai à próxima iteração
        if ((*nextBuffer)->isEmpty)
        {
            continue;
        }

        *nextThread = &((*nextBuffer)->queue[(*nextBuffer)->start]);

        return;
    }
}

void initializeScheduler(void)
{
    for (int i = 0; i < SCHEDULER_SIZE; i++)
    {
        initBuffer(&(scheduler.buffers[i]), quantumData[i]);
    }

    thread_create(NULL, main, NULL);

    getNextThread(&scheduler, &curr_buffer, &curr_tcb);

    tid = curr_tcb->tid;

    return;
}

void resetThreadPriorities(void)
{
    resetPriorityCount = 0;
    // move all threads to the highest priority queue
    for (int i = 0; i < SCHEDULER_SIZE; i++)
    {
        Buffer *bufferPtr = &scheduler.buffers[i];

        if (bufferPtr->isEmpty)
            continue;

        for (int j = 0; j < BUFFER_SIZE; j++)
        {
            tcb_t thread;
            if (bufferPtr->isEmpty)
                break;
            dequeue(bufferPtr, &thread);
            thread.priority = SCHEDULER_SIZE - 1;
            enqueue(&scheduler.buffers[0], &thread);
        }
    }
}

/**
 * Escalador: escolhe o próximo thread.
 *
 * @param enqueueAgain Boolean que indica se a thread atual deve ou não ser reenfileirada
 * @param wasPreempted Boolean que indica se a thread atual foi interrompida por temporizador (true) ou por yield (false)
 */
void schedule(bool enqueueAgain, bool wasPreempted)
{
    disableTimer0();

    // move current thread to the end of the queue
    tcb_t prev_tcb;

    if (!dequeue(curr_buffer, &prev_tcb))
        stop();

    uint32_t timerData = getTimer0Data();

    if (wasPreempted)
    {
        resetPriorityCount += timerData;
        prev_tcb.priority = prev_tcb.priority == 0 ? 0 : prev_tcb.priority - 1;
        prev_tcb.timerCount = 0;
    }
    else
    {
        uint32_t timerDiff = timerData - getTimer0Count();
        prev_tcb.timerCount += timerDiff;
        resetPriorityCount += timerDiff;

        if (prev_tcb.timerCount > timerData)
        {
            prev_tcb.priority = prev_tcb.priority == 0 ? 0 : prev_tcb.priority - 1;
            prev_tcb.timerCount = 0;
        }
    }

    if (enqueueAgain)
    {
        Buffer *bufferToEnqueueIn = &scheduler.buffers[SCHEDULER_SIZE - 1 - prev_tcb.priority];

        if (!enqueue(bufferToEnqueueIn, &prev_tcb))
            stop();
    }

    // tratamento da prioridade
    if (resetPriorityCount >= resetPriorityData)
    {
        resetThreadPriorities();
    }

    // obtém a próxima thread a ser executada (salvando em curr_tcb)
    getNextThread(&scheduler, &curr_buffer, &curr_tcb);

    tid = curr_tcb->tid;

    setDisplayNumber(tid);
    setLedsValue(curr_tcb->priority);

    setTimer0Data(ONE_SECOND * curr_buffer->quantumData);

    enableTimer0();
    resetTimer0Count();

    return;
}
