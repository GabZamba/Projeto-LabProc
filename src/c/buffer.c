#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h> // Includes the header of the memset function
#include "buffer.h"

/* Initializes the Buffer */
void initBuffer(Buffer *buffer, uint8_t quantumSize)
{
    buffer->start = NULL;
    buffer->end = NULL;
    buffer->quantumSize = quantumSize;

    return;
}

/* Enqueues the given tcb on the buffer */
bool enqueue(Buffer *buffer, tcb_t *addedThread)
{
    tcb_t *aux;

    addedThread->next = NULL;

    // if buffer is empty, enqueues at first position
    if (buffer->start == NULL)
    {
        buffer->start = addedThread;
        buffer->end = addedThread;
        return true;
    }

    // inserts at the end of the queue
    aux = buffer->end;
    aux->next = addedThread;
    buffer->end = addedThread;

    return true;
}

/* Dequeues the buffer and saves the value on the given tcb */
bool dequeue(Buffer *buffer, tcb_t **removedThread)
{
    if (buffer->start == NULL)
        return false;

    *removedThread = buffer->start;

    // dequeues from buffer
    buffer->start = buffer->start->next;

    (*removedThread)->next = NULL;

    return true;
}
