
#include <stdint.h>
#include <string.h> // Includes the header of the memset function
#include <stdio.h>
#include <stdlib.h>
#include "buffer.h"
#include "threads.h"

extern Scheduler scheduler;

extern tcb_t *curr_tcb; // current thread tcb

volatile uint32_t threadIdCounter = 0;
uint32_t nextTID(void) { return threadIdCounter++; }

typedef struct ThreadReturn
{
    void *data;
    uint32_t tid;
    struct ThreadReturn *next;
} ThreadReturn;

typedef struct
{
    ThreadReturn *start;
    ThreadReturn *end;
} ThreadReturnList;

ThreadReturnList threadReturnList = {};

/**
 * Creates a new thread, and adds it to the execution queue
 *
 * @param threadId ID of the new thread (will be returned by the function)
 * @param routine routine which will be executed by the thread
 * @param args pointer to the function arguments
 */
void thread_create(uint32_t *threadId, ThreadProperties *threadProperties, void *(*routine)(void *), void *args)
{
    tcb_t *newThread = (tcb_t *)malloc(sizeof(tcb_t));
    *newThread = (tcb_t){};
    uint32_t tid = nextTID();

    uint8_t *stack = (uint8_t *)malloc(4096 * sizeof(uint8_t));

    newThread->regs[0] = (uint32_t)args;
    newThread->sp = (uint32_t)stack + 4096;
    newThread->lr = (uint32_t)thread_exit;
    newThread->pc = (uint32_t)routine;

    newThread->cpsr = threadProperties == NULL ? 0x10 : threadProperties->cpsr;

    newThread->priority = SCHEDULER_SIZE - 1;
    newThread->tid = tid;
    newThread->next = NULL;

    if (threadId != NULL)
        *threadId = tid;

    enqueue(&scheduler.buffers[0], newThread);
}

/**
 * Checks if there is an active thread with the given id
 *
 * @param threadId id of the thread to be found
 * @param thread the properties of the thread to be found (will be returned by this function)
 * @return true if thread has been found, false if not
 */
bool findActiveThreadById(uint32_t threadId, tcb_t *thread)
{
    Buffer currBuffer;
    tcb_t *aux;

    for (int i = 0; i < SCHEDULER_SIZE; i++)
    {
        // checks if buffer is not empty
        currBuffer = scheduler.buffers[i];
        if (currBuffer.start == NULL)
            continue;

        // try to find the thread on the current buffer
        for (aux = currBuffer.start; aux != NULL; aux = aux->next)
        {
            if (aux->tid == threadId)
            {
                if (thread != NULL) // returns thread tcb if given pointer is not NULL
                    *thread = *aux;
                return true;
            }
        }
    }

    *thread = (tcb_t){};
    return false;
}

/**
 * Destroys the current thread, scheduling the next one in the queue
 *
 * @param returnPointer pointer to the value to be returned by the thread
 */
void __attribute__((naked)) thread_exit(void *returnPointer)
{
    asm volatile(
        "mov r1, r0 \n\t" // saves the return pointer on R1
        "mov r0, #3 \n\t" // #3 is the value on swi to destroy a thread
        "swi #0     \n\t");
}

/**
 * Saves the return of a completed thread, if not NULL, on the threadReturnList
 */
void save_return_pointer(void *returnPointer)
{
    if (returnPointer == NULL)
        return;

    ThreadReturn *item = (ThreadReturn *)malloc(sizeof(ThreadReturn));

    item->data = returnPointer;
    item->tid = curr_tcb->tid;
    item->next = NULL;

    // if the linked list is empty
    if (threadReturnList.start == NULL)
    { // sets the start and end
        threadReturnList.start = item;
        threadReturnList.end = item;
    }
    else
    { // inserts in the end
        threadReturnList.end->next = item;
        threadReturnList.end = item;
    }
}

/**
 * Searches the threadReturnList for the return of the given thread
 */
void *findThreadReturn(uint32_t thread_id)
{
    ThreadReturn *ptr = threadReturnList.start;

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
 * Makes the current thread wait for the execution of the thread with the given id
 *
 * @param threadId thread to wait for the end of the execution
 * @param threadReturn pointer which will contain the return value of the thread
 */
void thread_join(uint32_t threadId, void **threadReturn)
{
    while (findActiveThreadById(threadId, NULL))
        thread_yield();

    *threadReturn = findThreadReturn(threadId);
    return;
}

/**
 * A simple mutex lock
 *
 * @param mutex address of the mutex
 */
extern void thread_mutex_lock(void *mutex);

/**
 * A simple mutex unlock
 *
 * @param mutex address of the mutex
 */
extern void thread_mutex_unlock(void *mutex);

/**
 * Returns the control to the thread scheduler
 */
void __attribute__((naked)) thread_yield(void)
{
    asm volatile(
        "push {lr}  \n\t"
        "mov r0, #1 \n\t"
        "swi #0     \n\t"
        "pop {pc}");
}

/**
 * Returns the id of the current thread
 */
uint8_t __attribute__((naked)) get_tid(void)
{
    asm volatile(
        "push {lr}  \n\t"
        "mov r0, #2 \n\t"
        "swi #0     \n\t"
        "pop {pc}");
}
