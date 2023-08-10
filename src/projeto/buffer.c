#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h> // Incluir o cabeçalho para a função memset
#include "buffer.h"

/* Initialize the Buffer */
void initBuffer(Buffer *buffer, uint8_t quantumData)
{
    buffer->start = 0;
    buffer->end = 0;
    buffer->isEmpty = true;
    buffer->isFull = false;
    buffer->quantumData = quantumData;
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
