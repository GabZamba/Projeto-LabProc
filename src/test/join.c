
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../c/threads.h"

void *func1(void *args)
{
   int *ptr = (int *)malloc(sizeof(int));
   *ptr = 30;
   return ptr;
}

void *func2(void *args)
{
   int *ptr = (int *)malloc(sizeof(int));
   *ptr = 25;
   thread_exit(ptr);
}

int *return1;
int *return2;

void *main(void *args)
{
   uint32_t tid3, tid4;
   thread_create(&tid3, NULL, func1, NULL); // tid 1
   thread_create(&tid4, NULL, func1, NULL); // tid 2

   thread_join(tid3, (void *)&return1);
   thread_join(tid4, (void *)&return2);

   return NULL;
}
