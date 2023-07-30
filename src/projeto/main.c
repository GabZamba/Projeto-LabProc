
#include <stdint.h>
#include "kernel.h"

int main(void)
{
   int i;
   for (i = 0; i < 1; i++)
   {
      for (i = 0; i < 5; i++)
         ;
      yield();
   }
}

int main2(void)
{
   int i, j;
   for (j = 0; j < 3; j++)
   {
      for (i = 0; i < 7; i++)
         ;
      yield();
   }
   yield();
}
