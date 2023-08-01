#ifndef BUFFER_H
#define BUFFER_H

#include <stdint.h>
// #include <stdio.h>
#include <stdbool.h>
// #include <string.h> // Incluir o cabeçalho para a função memset

#define BUFFER_SIZE 3

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
} tcb_t;

typedef struct
{
    bool isEmpty;
    bool isFull;
    uint32_t start;
    uint32_t end;
    tcb_t queue[BUFFER_SIZE];
} Buffer;

void initBuffer(Buffer *buffer);

bool enqueue(Buffer *buffer, volatile tcb_t *thread);

bool dequeue(Buffer *buffer, volatile tcb_t *thread);

#endif
