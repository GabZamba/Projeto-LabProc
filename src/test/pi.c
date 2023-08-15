
#include <stdio.h>
#include <stdlib.h>
#include "../c/threads.h"

#define MAXTHREADS 8

// Use 200 million steps
long num_steps = 20000;
double sum = 0.0;
int nThreads;
double pi;

void *computePi(void *pArg)
{
    double step, x;
    int i;
    int myNum = *((int *)pArg);

    step = 1.0 / (double)num_steps;

    for (i = myNum; i < num_steps; i += nThreads)
    {
        x = (i + 0.5) * step;
        sum += 4.0 / (1.0 + x * x);
    }
    return NULL;
}

void *main()
{
    int i;
    double x;
    double step;
    int tNum[MAXTHREADS];
    uint32_t tid[MAXTHREADS];

    step = 1.0 / (double)num_steps;
    nThreads = 8;

    for (i = 0; i < nThreads; i++)
    {
        tNum[i] = i;
        thread_create(&tid[i], NULL, computePi, &tNum[i]);
    }

    for (i = 0; i < nThreads; i++)
    {
        thread_join(tid[i], NULL);
    }

    pi = step * sum;
    return NULL;
}
