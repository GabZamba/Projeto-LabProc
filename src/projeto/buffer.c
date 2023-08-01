#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h> // Incluir o cabeçalho para a função memset

#define BUFFER_SIZE 5
#define SCHEDULER_SIZE 4 // baseado no total de prioridades

/**
 * Estrutura do
 * Thread control block (TCB).
 */
typedef struct
{
    uint32_t regs[13]; // r0-r12
    uint32_t sp;
    uint32_t lr;
    uint32_t pc;
    uint32_t cpsr;
    unsigned int priority : 2; // prioridade (de 0 a 3)
    uint8_t tid;               // thread id
} tcb_t;

typedef struct
{
    bool isEmpty;
    bool isFull;
    uint32_t start;
    uint32_t end;
    tcb_t queue[BUFFER_SIZE];
} Buffer;

typedef struct
{
    Buffer buffers[4];
} Scheduler;

/* Initialize the Buffer */
void initBuffer(Buffer *buffer)
{
    buffer->start = 0;
    buffer->end = 0;
    buffer->isEmpty = true;
    buffer->isFull = false;
}

/* Enqueues the given tcb on the buffer */
bool enqueue(Buffer *buffer, tcb_t *addedThread)
{
    if (buffer->isFull)
        return false;

    // enqueue
    buffer->queue[buffer->end] = *addedThread;

    // calculates new end position
    buffer->end = (buffer->end + 1) % BUFFER_SIZE;
    buffer->isEmpty = false;

    // checks if buffer is full
    if (buffer->start == buffer->end)
        buffer->isFull = true;

    return true;
}

/* Dequeues the buffer and saves the value on the given tcb */
bool dequeue(Buffer *buffer, tcb_t *removedThread)
{
    if (buffer->isEmpty)
        return false;

    // dequeue and save on pointer
    *removedThread = buffer->queue[buffer->start];
    buffer->queue[buffer->start] = (tcb_t){};

    // calculates new end position
    buffer->start = (buffer->start + 1) % BUFFER_SIZE;
    buffer->isFull = false;

    // checks if buffer is empty
    if (buffer->start == buffer->end)
        buffer->isEmpty = true;

    return true;
}
