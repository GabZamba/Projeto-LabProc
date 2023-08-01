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
void thread_create(uint8_t *threadId, const uint8_t *priority, void (*routine)(void *), void *args);


#endif
