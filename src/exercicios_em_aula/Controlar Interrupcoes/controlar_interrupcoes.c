#include <stdint.h>

uint32_t enable_irq(void)
{
    uint32_t ultimoBit;
    uint32_t aux;
    asm("mrs %0, cpsr \n\t"
        "bic %1, %0, #0x80 \n\t"
        "msr cpsr, %1 \n\t"
        "lsr %0, %0, #7 \n\t"
        "and %0, %0, #1 \n\t"
        : "=r"(ultimoBit), "=r"(aux));

    return ultimoBit;
}

uint32_t disable_irq(void)
{
    uint32_t ultimoBit;
    uint32_t aux;
    asm("mrs %0, cpsr \n\t"
        "orr %1, %0, #0x80 \n\t"
        "msr cpsr, %1 \n\t"
        "lsr %0, %0, #7 \n\t"
        "and %0, %0, #1 \n\t"
        : "=r"(ultimoBit), "=r"(aux));

    return ultimoBit;
}

uint32_t enable_fiq(void)
{
    uint32_t ultimoBit;
    uint32_t aux;
    asm("mrs %0, cpsr \n\t"
        "bic %1, %0, #0x40 \n\t"
        "msr cpsr, %1 \n\t"
        "lsr %0, %0, #6 \n\t"
        "and %0, %0, #1 \n\t"
        : "=r"(ultimoBit), "=r"(aux));

    return ultimoBit;
}

uint32_t disable_fiq(void)
{
    uint32_t ultimoBit;
    uint32_t aux;
    asm("mrs %0, cpsr \n\t"
        "orr %1, %0, #0x40 \n\t"
        "msr cpsr, %1 \n\t"
        "lsr %0, %0, #6 \n\t"
        "and %0, %0, #1 \n\t"
        : "=r"(ultimoBit), "=r"(aux));

    return ultimoBit;
}

int main()
{
    uint32_t ultimoBit = 0;
    ultimoBit = disable_irq();
    ultimoBit = enable_irq();
    ultimoBit = disable_fiq();
    ultimoBit = enable_fiq();
    return 0;
}