
#include <stdint.h>
#include <stdlib.h>
#include "threads.h"

uint32_t mutex;

void *func1(void *args)
{
   int a;
   while (true)
   {
      a += 1;
      int *ptr = (int *)malloc(sizeof(int));
      *ptr = 1;
      return ptr;
   }
}

void *func2(void *args)
{
   int a;
   while (true)
   {
      a += 1;
      int *ptr = (int *)malloc(sizeof(int));
      *ptr = 2;
      return ptr;
   }
}

void *main(void *args)
{
   uint32_t tid1;
   uint32_t tid2;
   thread_create(&tid1, 1, func1, NULL);
   thread_create(&tid2, 2, func2, NULL);
   int *return1;
   int *return2;

   thread_join(tid1, (void **)&return1);
   thread_join(tid2, (void **)&return2);

   int *ptr = (int *)malloc(sizeof(int));
   *ptr = 30;
   return ptr;
}
