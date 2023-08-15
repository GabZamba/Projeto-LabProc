
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../c/threads.h"

void *func1(void *args) {}

void *func3(void *args) {}

void *func2(void *args) { thread_create(NULL, NULL, func3, NULL); }

void *main(void *args)
{
   thread_create(NULL, NULL, func1, NULL); // tid 1
   thread_create(NULL, NULL, func2, NULL); // tid 2
   thread_create(NULL, NULL, func1, NULL); // tid 3

   return NULL;
}
