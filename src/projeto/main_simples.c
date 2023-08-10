
#include <stdint.h>
#include <stdlib.h>
#include "threads.h"

uint32_t mutex;

void *func1(void *args)
{
   int a;
   while (true)
   {
      for (int i = 0; i < 200000; i++)
         ;
      yield();
   }
}

void *func2(void *args)
{
   int a;
   while (true)
   {
   }
}

void *main(void *args)
{
   thread_create(NULL, func1, NULL);
   thread_create(NULL, func2, NULL);
   thread_create(NULL, func1, NULL);

   return NULL;
}
