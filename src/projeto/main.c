
#include <stdint.h>
#include "kernel.h"

int main(void)
{
   int i, j;
   for (i = 0; i < 2; i++)
   {
      for (j = 0; j < 5; j++)
         ;
      yield();
   }
}

int main2(void)
{
   int i, j;
   for (i = 0; i < 3; i++)
   {
      for (j = 0; j < 7; j++)
         ;
      yield();
   }
   yield();
}
