#ifndef THREADS_H
#define THREADS_H

#include <stdint.h>
#include "buffer.h"

/**
 * Creates a new thread, and adds it to the execution queue
 *
 * @param threadId ID of the new thread (will be returned by the function)
 * @param routine routine which will be executed by the thread
 * @param args pointer to the function arguments
 */
void thread_create(uint32_t *threadId, void *(*routine)(void *), void *args);

/**
 * Destroys the current thread, scheduling the next one in the queue
 *
 * @param returnPointer pointer to the value to be returned by the thread
 */
void thread_exit(void *returnPointer);

/**
 * Makes the current thread wait for the execution of the thread with the given id
 *
 * @param threadId thread to wait for the end of the execution
 * @param threadReturn pointer which will contain the return value of the thread
 */
void thread_join(uint32_t threadId, void **threadReturn);

/**
 * A simple mutex lock
 *
 * @param mutex address of the mutex
 */
void thread_mutex_lock(void *mutex);

/**
 * A simple mutex unlock
 *
 * @param mutex address of the mutex
 */
void thread_mutex_unlock(void *mutex);

/**
 * Returns the control to the thread scheduler, scheduling the next one in the queue
 */
void thread_yield(void);

/**
 * Retorns the Id of the current thread
 */
uint8_t get_tid(void);

#endif
