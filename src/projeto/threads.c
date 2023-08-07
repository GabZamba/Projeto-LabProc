
#include <stdint.h>
#include <string.h> // Incluir o cabeçalho para a função memset
#include "buffer.h"
#include "threads.h"

// Definidos pelo linker:
extern uint8_t stack_svr[];
extern uint8_t stack_usr[];

extern Scheduler scheduler;

volatile uint32_t count = 0;
uint32_t nextTID(void) { return count++; }

/**
 * Cria uma nova thread, e a adiciona na fila de execução
 *
 * @param threadId O ID da thread (será retornado por esta)
 * @param priority A prioridade da thread (de 0 a 3, imutável)
 * @param routine A função que a thread executará
 * @param args Os parâmetros que serão passados à função
 */
void thread_create(uint8_t *threadId, const uint8_t *priority, void *(*routine)(void *), void *args)
{
    tcb_t newThread = {0};
    uint8_t tid = nextTID();

    const uint8_t threadPriority = (priority == NULL) ? 0 : *priority;

    newThread.regs[0] = (uint32_t)args;
    newThread.sp = (uint32_t)stack_usr + (4096 * tid);
    newThread.lr = (uint32_t)thread_exit;
    newThread.pc = (uint32_t)routine;

    newThread.cpsr = 0x10;

    newThread.priority = threadPriority;
    newThread.tid = tid;

    *threadId = tid;

    Buffer *buffer = &scheduler.buffers[SCHEDULER_SIZE - 1 - threadPriority];

    enqueue(buffer, &newThread);
}

/**
 * Verifica se há uma thread ativa com o threadId informado
 *
 * @param threadId O ID da thread
 * @param thread Os valores da thread serão retornados aqui
 * @return true se encontrou thread, false se não encontrou
 */
bool getThreadById(uint8_t threadId, tcb_t *thread)
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
        "mov r0, #3 \n\t"
        "swi #0     \n\t");
}

/**
 * Faz a thread atual esperar o término da execução da thread com id thread_id
 */
void thread_join(uint8_t thread_id)
{
    tcb_t threadToWait;
    while (getThreadById(thread_id, &threadToWait))
    {
        yield();
    };
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
