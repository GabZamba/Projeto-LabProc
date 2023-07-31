
#include <stdint.h>
#include "kernel.h"

int func1(void) {}

int func2(void) {}

int main(void)
{
   int i, j = 0;
   for (i = 0; i < 2; i++)
   {
      for (j = 0; j < 5; j++)
         ;
      yield();
   }
   func1();
}
