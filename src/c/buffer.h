#ifndef BUFFER_H
#define BUFFER_H

#include <stdint.h>
#include <stdbool.h>

#define BUFFER_SIZE 10
#define SCHEDULER_SIZE 4 // based on number of priorities

/**
 * Thread Control Block (TCB) structure
 */
typedef struct
{
    uint32_t regs[13]; // r0-r12
    uint32_t sp;
    uint32_t lr;
    uint32_t pc;
    uint32_t cpsr;
    uint8_t priority : 2; // priority, 0 (min) to 3 (max)
    uint8_t tid;          // thread id
    uint64_t timerCount;
} tcb_t;

typedef struct
{
    bool isEmpty;
    bool isFull;
    uint32_t start;
    uint32_t end;
    tcb_t *queue[BUFFER_SIZE]; // array of pointers
    uint8_t quantumSize;
} Buffer;

typedef struct
{
    Buffer buffers[SCHEDULER_SIZE];
} Scheduler;

/**
 * Initializes the Buffer
 *
 * @param buffer pointer to the buffer which will be initialized
 * @param quantumSize the size of the quantum which will be initialized
 */
void initBuffer(Buffer *buffer, uint8_t quantumSize);

/**
 * Enqueues the given thread on the buffer
 *
 * @param buffer pointer to the buffer in which the thread will be enqueued
 * @param addedThread pointer to the thread which will be enqueued
 */
bool enqueue(Buffer *buffer, tcb_t *addedThread);

/**
 * Dequeues the buffer and saves the value on the given thread pointer
 *
 * @param buffer pointer to the buffer in which the first thread will be dequeued
 * @param removedThread pointer which will contain the dequeued thread
 */
bool dequeue(Buffer *buffer, tcb_t **removedThread);

#endif
