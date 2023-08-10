
#include <stdint.h>
#include <string.h> // Incluir o cabeçalho para a função memset
#include <stdio.h>
#include <stdlib.h>
#include "buffer.h"
#include "threads.h"

extern Scheduler scheduler;

extern tcb_t *curr_tcb; // tcb do thread atual

volatile uint32_t threadIdCounter = 0;
uint32_t nextTID(void) { return threadIdCounter++; }

typedef struct ThreadReturnItem
{
    void *data;
    uint32_t tid;
    struct ThreadReturnItem *prev;
    struct ThreadReturnItem *next;
} ThreadReturnItem;

typedef struct
{
    ThreadReturnItem *start;
    ThreadReturnItem *end;
} ThreadReturnList;

ThreadReturnList threadReturnList = {};

/**
 * Cria uma nova thread, e a adiciona na fila de execução
 *
 * @param threadId O ID da thread (será retornado por esta)
 * @param priority A prioridade da thread (de 0 a 3, imutável)
 * @param routine A função que a thread executará
 * @param args Os parâmetros que serão passados à função
 */
void thread_create(uint32_t *threadId, void *(*routine)(void *), void *args)
{
    tcb_t newThread = {0};
    uint32_t tid = nextTID();

    uint8_t *stack = (uint8_t *)malloc(4096 * sizeof(uint8_t));

    newThread.regs[0] = (uint32_t)args;
    newThread.sp = (uint32_t)stack + 4096;
    newThread.lr = (uint32_t)thread_exit;
    newThread.pc = (uint32_t)routine;

    newThread.cpsr = 0x10;

    newThread.priority = SCHEDULER_SIZE - 1;
    newThread.tid = tid;

    *threadId = tid;

    enqueue(&scheduler.buffers[0], &newThread);
}

/**
 * Verifica se há uma thread ativa com o threadId informado
 *
 * @param threadId O ID da thread
 * @param thread Os valores da thread serão retornados aqui
 * @return true se encontrou thread, false se não encontrou
 */
bool getThreadById(uint32_t threadId, tcb_t *thread)
{
    Buffer currBuffer;
    for (int i = 0; i < SCHEDULER_SIZE; i++)
    {
        currBuffer = scheduler.buffers[i];
        if (currBuffer.isEmpty)
            continue;
        // itera sobre o buffer
        for (int j = 0; j < BUFFER_SIZE; j++)
        {
            // retorna se o tid for igual
            if (currBuffer.queue[j].tid == threadId)
            {
                if (thread != NULL) // retorna o valor se o ponteiro não for nulo
                    *thread = currBuffer.queue[j];
                return true;
            }
        }
    }
    return false;
}

/**
 * Destrói a thread atual, executando a próxima na fila.
 */
void __attribute__((naked)) thread_exit(void)
{
    asm volatile(
        "mov r1, r0 \n\t" // salva o ponteiro retornado em R1
        "mov r0, #3 \n\t" // #3 é o valor para sair de uma thread pelo swi
        "swi #0     \n\t");
}

void save_return_pointer(void *returnPointer)
{
    if (returnPointer == NULL)
        return;

    ThreadReturnItem *item = (ThreadReturnItem *)malloc(sizeof(ThreadReturnItem));
    item->data = returnPointer;
    item->tid = curr_tcb->tid;
    item->next = NULL;
    item->prev = NULL;

    // se a lista está vazia
    if (threadReturnList.start == NULL)
    { // define início e fim da lista
        threadReturnList.start = item;
        threadReturnList.end = item;
    }
    else
    { // insere no final da lista
        item->prev = threadReturnList.end;
        threadReturnList.end->next = item;
        threadReturnList.end = item;
    }
}

/**
 * Procura na lista de retornos de thread o ponteiro retornado pela thread especificada
 *
 * @param thread_id O ID da thread
 */
void *findThreadReturn(uint32_t thread_id)
{
    ThreadReturnItem *ptr = threadReturnList.start;
    if (ptr == NULL)
        return NULL;
    while (ptr != NULL)
    {
        if (ptr->tid == thread_id)
            return ptr->data;
        else
            ptr = ptr->next;
    }
    return NULL;
}

/**
 * Faz a thread atual esperar o término da execução da thread com id thread_id
 *
 * @param thread_id O ID da thread
 * @param thread_return Ponteiro para o valor retornado pela thread passada
 */
void thread_join(uint32_t thread_id, void **thread_return)
{
    while (getThreadById(thread_id, NULL))
    {
        yield();
    };
    *thread_return = findThreadReturn(thread_id);
    return;
}

/**
 * Chama o kernel com swi, a função "yield" (r0 = 1).
 * Devolve o controle ao sistema executivo, que pode escalar outro thread.
 */
void __attribute__((naked)) yield(void)
{
    asm volatile(
        "push {lr}  \n\t"
        "mov r0, #1 \n\t"
        "swi #0     \n\t"
        "pop {pc}");
}

/**
 * Retorna o thread-id do thread atual.
 */
int __attribute__((naked)) getpid(void)
{
    asm volatile(
        "push {lr}  \n\t"
        "mov r0, #2 \n\t"
        "swi #0     \n\t"
        "pop {pc}");
}
