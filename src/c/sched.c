
#include <stdint.h>
#include <string.h> // Includes the header of the memset function
#include "gpio_utils.h"
#include "buffer.h"
#include "threads.h"
#include "stdlib.h"

Scheduler scheduler = {};

// first thread to be executed (user main function)
extern void *main(void *);

void stop(void); // stop label on interrupt.s

volatile int tid; // current thread id
tcb_t *curr_tcb;  // current thread tcb
Buffer *curr_buffer;

volatile uint64_t resetPriorityCount = 0;
volatile uint64_t resetPriorityData = (uint64_t)30 * ONE_SECOND;

uint8_t quantumData[SCHEDULER_SIZE] = {1, 2, 3, 4};

/**
 * Gets the next thread to be executed
 *
 * @param nextBuffer pointer to the next buffer (will be defined by this function)
 * @param nextThread pointer to the next thread (will be defined by this function)
 */
void getNextThread(Buffer **nextBuffer, tcb_t **nextThread)
{
    Buffer *buffers = &(scheduler.buffers[0]);

    // get the next thread
    for (int i = 0; i < SCHEDULER_SIZE; i++)
    {
        // find the first non-empty buffer
        *nextBuffer = &buffers[i];
        if ((*nextBuffer)->start == NULL)
            continue;

        *nextThread = (*nextBuffer)->start;
        return;
    }

    // if all buffers are empty, the next thread will return to reset (pc = lr = 0)
    ThreadProperties tp = (ThreadProperties){cpsr : 0x13};
    thread_create(NULL, &tp, NULL, NULL);

    *nextBuffer = &buffers[0];
    *nextThread = (*nextBuffer)->start;
    return;
}

void initializeScheduler(void)
{
    for (int i = 0; i < SCHEDULER_SIZE; i++)
        initBuffer(&(scheduler.buffers[i]), quantumData[i]);

    thread_create(NULL, NULL, main, NULL);

    getNextThread(&curr_buffer, &curr_tcb);

    tid = curr_tcb->tid;

    return;
}

void resetThreadPriorities(void)
{
    Buffer *bufferPtr;
    tcb_t *thread;

    resetPriorityCount = 0;
    // move all threads to the highest priority queue
    for (int i = 0; i < SCHEDULER_SIZE; i++)
    {
        bufferPtr = &scheduler.buffers[i];

        if (bufferPtr->start == NULL)
            continue;

        while (dequeue(bufferPtr, &thread))
        {
            thread->priority = SCHEDULER_SIZE - 1; // sets priority to max
            enqueue(&scheduler.buffers[0], thread);
        }
    }
}

/**
 * Scheduler: chooses next executing thread
 *
 * @param enqueueAgain indicates if current thread will be requeued
 * @param wasPreempted indicates if current thread has been interrupted by timer (true), or has yielded control (false)
 */
void schedule(bool enqueueAgain, bool wasPreempted)
{
    disableTimer0();

    tcb_t *prev_tcb;
    uint32_t timerData, timerDiff;

    // dequeues current thread and saves on prev_tcb
    if (!dequeue(curr_buffer, &prev_tcb))
        stop();

    timerData = getTimer0Data();
    timerDiff = wasPreempted ? timerData : (timerData - getTimer0Count());

    // if has already used one quantum of time, decreases the priority
    prev_tcb->timerCount += timerDiff;
    if (prev_tcb->timerCount >= timerData)
    {
        if (prev_tcb->priority > 0) // decreases if not already at minimum
            prev_tcb->priority--;
        prev_tcb->timerCount = 0;
    }

    if (!enqueueAgain)
    {
        free(prev_tcb);
    }
    else
    {
        // max priority = index 0, min priority = index 3
        uint32_t bufferIndex = SCHEDULER_SIZE - 1 - prev_tcb->priority;
        Buffer *bufferToEnqueueIn = &scheduler.buffers[bufferIndex];

        if (!enqueue(bufferToEnqueueIn, prev_tcb))
            stop();
    }

    // treat the priority reset
    resetPriorityCount += timerDiff;
    if (resetPriorityCount >= resetPriorityData)
        resetThreadPriorities();

    // gets next thread to be executed, saving on curr_tcb
    getNextThread(&curr_buffer, &curr_tcb);

    tid = curr_tcb->tid;

    changeDisplayNumber(tid);
    changeLedsValue(curr_tcb->priority);

    // sets the quantum size (based on the current buffer)
    setTimer0Data(ONE_SECOND * curr_buffer->quantumSize);

    enableTimer0();
    resetTimer0Count();

    return;
}
