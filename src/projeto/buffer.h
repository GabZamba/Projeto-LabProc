#ifndef BUFFER_H
#define BUFFER_H

#include <stdint.h>
#include <stdbool.h>

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
    uint8_t priority : 2; // prioridade (de 0 a 3)
    uint8_t tid;          // thread id
    uint64_t timerCount;
} tcb_t;

typedef struct
{
    bool isEmpty;
    bool isFull;
    uint32_t start;
    uint32_t end;
    tcb_t queue[BUFFER_SIZE];
    uint8_t quantumSize;
} Buffer;

typedef struct
{
    Buffer buffers[SCHEDULER_SIZE];
} Scheduler;

void initBuffer(Buffer *buffer, uint8_t quantumSize);

bool enqueue(Buffer *buffer, tcb_t *addedThread);

bool dequeue(Buffer *buffer, tcb_t *removedThread);

#endif
