#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h> // Includes the header of the memset function
#include "buffer.h"

/* Initializes the Buffer */
void initBuffer(Buffer *buffer, uint8_t quantumSize)
{
    buffer->start = 0;
    buffer->end = 0;
    buffer->isEmpty = true;
    buffer->isFull = false;
    buffer->quantumSize = quantumSize;

    return;
}

/* Enqueues the given tcb on the buffer */
bool enqueue(Buffer *buffer, tcb_t *addedThread)
{
    if (buffer->isFull)
        return false;

    // enqueues the thread
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

    // saves previous thread on pointer
    *removedThread = buffer->queue[buffer->start];
    // resets memory of the previous thread
    buffer->queue[buffer->start] = (tcb_t){};

    // calculates new start position
    buffer->start = (buffer->start + 1) % BUFFER_SIZE;
    buffer->isFull = false;

    // checks if buffer is empty
    if (buffer->start == buffer->end)
        buffer->isEmpty = true;

    return true;
}
