#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h> // Incluir o cabeçalho para a função memset

#define BUFFER_SIZE 5

/**
 * Estrutura do
 * Thread control block (TCB).
 */
typedef struct
{
    uint32_t regs[17];         // 16 registradores + cpsr
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

void initBuffer(Buffer *buffer)
{
    buffer->start = 0;
    buffer->end = 0;
    buffer->isEmpty = true;
    buffer->isFull = false;
}

bool enqueue(Buffer *buffer, volatile tcb_t *thread)
{
    if (buffer->isFull)
        return false;

    // enqueue
    buffer->queue[buffer->end] = *thread;

    // calculates new end position
    buffer->end = (buffer->end + 1) % BUFFER_SIZE;
    buffer->isEmpty = false;

    // checks if buffer is full
    if (buffer->start == buffer->end)
        buffer->isFull = true;

    return true;
}

bool dequeue(Buffer *buffer, volatile tcb_t *thread)
{
    if (buffer->isEmpty)
        return false;

    // dequeue and save on pointer
    *thread = buffer->queue[buffer->start];

    // calculates new end position
    buffer->start = (buffer->start + 1) % BUFFER_SIZE;
    buffer->isFull = false;

    // checks if buffer is empty
    if (buffer->start == buffer->end)
        buffer->isEmpty = true;

    return true;
}
