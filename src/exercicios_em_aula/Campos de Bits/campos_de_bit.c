#include <stdio.h>
#include <stdint.h>
#include "evlt7t.h"
#include <string.h> // Incluir o cabeçalho para a função memset

const int posicaoIopcon = 0x40100000; // QEMU
// const int posicaoIopcon = 0x03ff5004; // Evaluator7T

struct __attribute__((packed)) IOPCON_t
{

    unsigned int EIRQ0_interruptEvent : 2;
    unsigned int EIRQ0_debounce : 1;
    unsigned int EIRQ0_verifyLevel : 1;
    unsigned int EIRQ0_enable : 1;

    unsigned int EIRQ1_interruptEvent : 2;
    unsigned int EIRQ1_debounce : 1;
    unsigned int EIRQ1_verifyLevel : 1;
    unsigned int EIRQ1_enable : 1;

    unsigned int EIRQ2_interruptEvent : 2;
    unsigned int EIRQ2_debounce : 1;
    unsigned int EIRQ2_verifyLevel : 1;
    unsigned int EIRQ2_enable : 1;

    unsigned int EIRQ3_interruptEvent : 2;
    unsigned int EIRQ3_debounce : 1;
    unsigned int EIRQ3_verifyLevel : 1;
    unsigned int EIRQ3_enable : 1;

    unsigned int DMA_REQ0_level : 1;
    unsigned int DMA_REQ0_debounce : 1;
    unsigned int DMA_REQ0_enable : 1;

    unsigned int DMA_REQ1_level : 1;
    unsigned int DMA_REQ1_debounce : 1;
    unsigned int DMA_REQ1_enable : 1;

    unsigned int DMA_ACK0_level : 1;
    unsigned int DMA_ACK0_enable : 1;

    unsigned int DMA_ACK1_level : 1;
    unsigned int DMA_ACK1_enable : 1;

    unsigned int TOUT_EN_timer0 : 1;
    unsigned int TOUT_EN_timer1 : 1;
};

struct IOPCON_t *iopcon = (struct IOPCON_t *)posicaoIopcon;
int32_t *iopconInt = (int32_t *)posicaoIopcon;

int main()
{
    *iopconInt = 0; // reset

    iopcon->EIRQ0_enable = 1;
    iopcon->EIRQ0_interruptEvent = 3;
    iopcon->EIRQ0_verifyLevel = 1;
    iopcon->EIRQ0_debounce = 1;

    iopcon->EIRQ1_enable = 1;
    iopcon->EIRQ1_interruptEvent = 3;
    iopcon->EIRQ1_verifyLevel = 1;
    iopcon->EIRQ1_debounce = 1;

    iopcon->EIRQ2_enable = 1;
    iopcon->EIRQ2_interruptEvent = 3;
    iopcon->EIRQ2_verifyLevel = 1;
    iopcon->EIRQ2_debounce = 1;

    iopcon->EIRQ3_enable = 1;
    iopcon->EIRQ3_interruptEvent = 3;
    iopcon->EIRQ3_verifyLevel = 1;
    iopcon->EIRQ3_debounce = 1;

    return 0;
}