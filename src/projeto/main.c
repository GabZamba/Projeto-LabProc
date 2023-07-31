
#include <stdint.h>
#include "kernel.h"

int i, j = 0;

int main(void)
{
   // int i, j;
   // for (i = 0; i < 2; i++)
   // {
   //    for (j = 0; j < 5; j++)
   //       ;
   //    yield();
   // }
}

int mainSVR(void)
{
   for (i = 0; i < 2; i++)
   {
      for (j = 0; j < 5; j++)
         ;
      yield();
   }
   main();
}

int main2(void)
{
   // int i, j;
   // for (i = 0; i < 3; i++)
   // {
   //    for (j = 0; j < 7; j++)
   //       ;
   //    yield();
   // }
   // yield();
}
