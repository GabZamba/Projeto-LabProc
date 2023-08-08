
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

uint8_t QuantumData[SCHEDULER_SIZE] = {5, 4, 3, 2};

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
    bool allQuantumConsumed = true;

    Buffer *buffers = &scheduler->buffers[0];

    // verifica se todos os buffers estao vazios ou já tiveram seu quantum esgotado
    for (int i = 0; i < SCHEDULER_SIZE; i++)
    {
        // verifica se há buffers não-vazios, zerando os quantums dos vazios
        if (buffers[i].isEmpty)
            buffers[i].quantumCnt = 0;
        else
            allBuffersEmpty = false;
        // verifica se há quantums a serem executados
        if (buffers[i].quantumCnt != 0)
            allQuantumConsumed = false;
    }

    // se todos estiverem vazios, retorna ao reset (pc e lr são 0)
    if (allBuffersEmpty)
    {
        *nextBuffer = &buffers[0];
        *nextThread = &((*nextBuffer)->queue[(*nextBuffer)->start]);
        (*nextThread)->cpsr = 0x13;
        return;
    }

    // caso necessário, reinicia os quantumCnt
    if (allQuantumConsumed)
    {
        for (int i = 0; i < SCHEDULER_SIZE; i++)
        {
            buffers[i].quantumCnt = buffers[i].quantumData;
        }
    }

    // pega a proxima thread
    for (int i = 0; i < SCHEDULER_SIZE; i++)
    {
        // itera pelo scheduler, selecionando o primeiro buffer que não está vazio
        *nextBuffer = &buffers[i];

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

    thread_create(NULL, 0, main, NULL);

    getNextThread(&scheduler, &curr_buffer, &curr_tcb);

    tid = curr_tcb->tid;

    return;
}

/**
 * Escalador: escolhe o próximo thread.
 * 
 * @param enqueueAgain Boolean que indica se a thread atual deve ou não ser reenfileirada
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

    // obtém a próxima thread a ser executada (salvando em curr_tcb)
    getNextThread(&scheduler, &curr_buffer, &curr_tcb);

    tid = curr_tcb->tid;

    setDisplayNumber(tid);
    setLedsValue(curr_tcb->priority);

    enableTimer0();
    resetTimer0Count();

    return;
}
