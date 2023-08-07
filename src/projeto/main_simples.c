
#include <stdint.h>
#include "threads.h"

uint32_t mutex;

void *func1(void *args)
{
   int a;
   while (true)
   {
      a += 1;
      yield();
   }
}

void *func2(void *args)
{
   int a;
   while (true)
   {
      a += 1;
      yield();
   }
}

void *main(void *args)
{
   thread_create(NULL, NULL, func1, NULL);
   thread_create(NULL, NULL, func2, NULL);
   return NULL;
}
