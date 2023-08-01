
#include <stdint.h>
#include <string.h> // Incluir o cabeçalho para a função memset
#include "buffer.h"

// Definidos pelo linker:
extern uint8_t stack_svr[];
extern uint8_t stack_usr[];

void destroy_thread(void); // declaracao da funcao final de cada thread

extern Scheduler scheduler;

uint32_t count = 0;
uint32_t nextTID(void) { return count++; }

/**
 * Cria uma nova thread, e a adiciona na fila de execução
 *
 * @param threadId O ID da thread (será retornado por esta)
 * @param priority A prioridade da thread (de 0 a 3, imutável)
 * @param routine A função que a thread executará
 * @param args Os parâmetros que serão passados à função
 */
void thread_create(uint8_t *threadId, const uint8_t *priority, void (*routine)(void *), void *args)
{
    tcb_t newThread = {0};
    uint8_t tid = nextTID();

    const uint8_t threadPriority = (priority == NULL) ? 0 : *priority;

    newThread.regs[0] = (uint32_t)args;
    newThread.sp = (uint32_t)stack_usr + (4096 * tid);
    newThread.lr = (uint32_t)destroy_thread;
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
        if (currBuffer.isEmpty) continue;
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
