
#include <stdint.h>
#include <stdlib.h>
#include "threads.h"

volatile uint32_t a;

void *func3(void *args)
{
   // for (int i = 0; i < 2000000; i++)
   //    ;
   // uint32_t *ptr = malloc(sizeof(uint32_t));
   // *ptr = ++a;
   // thread_exit(ptr);
}

void *func1(void *args)
{
   while (true)
   {
      for (int j = 0; j < 10; j++)
      {
         for (int i = 0; i < 200000; i++)
            ;
         thread_yield();
      }
      // return NULL;
      // thread_create(NULL, func3, NULL);
   }
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
