#ifndef THREADS_H
#define THREADS_H

#include <stdint.h>
#include <string.h> // Incluir o cabeçalho para a função memset
#include "buffer.h"

/**
 * Cria uma nova thread, e a adiciona na fila de execução
 *
 * @param threadId O ID da thread (será retornado por esta)
 * @param priority A prioridade da thread (de 0 a 3, imutável)
 * @param routine A função que a thread executará
 * @param args Os parâmetros que serão passados à função
 */

void thread_create(uint32_t *threadId, void *(*routine)(void *), void *args);

/**
 * Verifica se há uma thread ativa com o threadId informado
 *
 * @param threadId O ID da thread
 * @param thread Os valores da thread serão retornados aqui
 * @return true se encontrou thread, false se não encontrou
 */
bool getThreadById(uint32_t threadId, tcb_t *thread);

/**
 * Destrói a thread atual, executando a próxima na fila.
 */
void thread_exit(void);

/**
 * Faz a thread atual esperar o término da execução da thread com id thread_id
 *
 * @param thread_id O ID da thread
 * @param thread_return Ponteiro para o valor retornado pela thread passada
 */
void thread_join(uint32_t thread_id, void **thread_return);

extern void thread_mutex_lock(void *mutex);

extern void thread_mutex_unlock(void *mutex);

// Chamadas do kernel

/**
 * Devolve o controle ao sistema executivo, que pode escalar outro thread.
 */
void yield(void);

/**
 * Retorna o threadId da thread atual
 */
int getpid(void);

#endif
