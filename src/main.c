
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "c/threads.h"

void *func1(void *args)
{
   while (true)
      thread_yield();
}

void *func2(void *args)
{
   while (true)
      ;
}

void *main(void *args)
{
   thread_create(NULL, func1, NULL); // tid 1
   thread_create(NULL, func2, NULL); // tid 2
   thread_create(NULL, func1, NULL); // tid 3

   return NULL;
}
