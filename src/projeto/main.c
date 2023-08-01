
#include <stdint.h>
#include "kernel.h"
#include "threads.h"

void func1(void *args) {}

void func2(void *args) {}

int main(void)
{
   const int numThreads = 3;
   uint8_t threadIds[3];

   int i = 0;
   for (i = 0; i < numThreads; i++)
   {
      thread_create(&threadIds[i], NULL, func2, NULL);
   }
   func1(NULL);

   tcb_t thread;
   for (i = 0; i < numThreads; i++)
   {
      getThreadById(threadIds[i], &thread);
   }

   yield();
}
