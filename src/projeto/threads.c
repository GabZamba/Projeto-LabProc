
#include <stdint.h>
#include <string.h> // Incluir o cabeçalho para a função memset
#include "buffer.h"

// Definidos pelo linker:
extern uint8_t stack_svr[];
extern uint8_t stack_usr[];

void destroy_thread(void); // declaracao da funcao final de cada thread

extern Buffer buffer;

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

    newThread.regs[0] = (uint32_t)args;
    newThread.sp = (uint32_t)stack_usr + (4096 * tid);
    newThread.lr = (uint32_t)destroy_thread;
    newThread.pc = (uint32_t)routine;

    newThread.cpsr = 0x10;

    newThread.priority = (priority == NULL) ? 0 : *priority;
    newThread.tid = tid;

    *threadId = tid;

    enqueue(&buffer, &newThread);
}
