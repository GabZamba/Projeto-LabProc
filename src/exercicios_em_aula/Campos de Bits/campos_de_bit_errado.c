#include <stdio.h>
#include <stdint.h>
#include "evlt7t.h"
#include <string.h> // Incluir o cabeçalho para a função memset

const int posicaoIopcon = 0x40100000; // QEMU
// const int posicaoIopcon = 0x03ff5004; // Evaluator7T

struct __attribute__((packed)) ExternalInterrupt
{
    /*
    Evento a identificar como interrupção
        00 Nível (conforme bit 1)
        01 Borda de subida
        10 Borda de descida
        11 Qualquer transição
    */
    unsigned int interruptEvent : 2;
    /* Ativar debounce (1) */
    unsigned int debounce : 1;
    /* Nível para verificar interrupção */
    unsigned int verifyLevel : 1;
    /* Habilita a interrupção (1) */
    unsigned int enable : 1;
};

struct __attribute__((packed)) DirectMemoryAccessRequest
{
    /* Ativo alto (1) ou baixo (0) */
    unsigned int level : 1;
    /* Ativar debounce (1) */
    unsigned int debounce : 1;
    /* Habilita sinal DMA request (1) */
    unsigned int enable : 1;
};

struct __attribute__((packed)) DirectMemoryAccessAcknowledge
{
    /* Ativo alto (1) ou baixo (0) */
    unsigned int level : 1;
    /* Habilita sinal DMA acknowledge (1) */
    unsigned int enable : 1;
};

struct __attribute__((packed)) TimerOutputEnable
{
    /* habilita (1) saída do timer 0 */
    unsigned int timer0 : 1;
    /* habilita (1) saída do timer 1 */
    unsigned int timer1 : 1;
};

struct __attribute__((packed)) IOPCON_t
{
    struct ExternalInterrupt EIRQ0;
    struct ExternalInterrupt EIRQ1;
    struct ExternalInterrupt EIRQ2;
    struct ExternalInterrupt EIRQ3;
    struct DirectMemoryAccessRequest DMA_REQ0;
    struct DirectMemoryAccessRequest DMA_REQ1;
    struct DirectMemoryAccessAcknowledge DMA_ACK0;
    struct DirectMemoryAccessAcknowledge DMA_ACK1;
    struct TimerOutputEnable TOUT_EN;
};

struct IOPCON_t *iopcon = (struct IOPCON_t *)posicaoIopcon;
int32_t *iopconInt = (int32_t *)posicaoIopcon;

int main()
{
    *iopconInt = 0; // reset

    iopcon->EIRQ0.enable = 1;
    iopcon->EIRQ0.interruptEvent = 3;
    iopcon->EIRQ0.verifyLevel = 1;
    iopcon->EIRQ0.debounce = 1;

    iopcon->EIRQ1.enable = 1;
    iopcon->EIRQ1.interruptEvent = 3;
    iopcon->EIRQ1.verifyLevel = 1;
    iopcon->EIRQ1.debounce = 1;

    iopcon->EIRQ2.enable = 1;
    iopcon->EIRQ2.interruptEvent = 3;
    iopcon->EIRQ2.verifyLevel = 1;
    iopcon->EIRQ2.debounce = 1;

    iopcon->EIRQ3.enable = 1;
    iopcon->EIRQ3.interruptEvent = 3;
    iopcon->EIRQ3.verifyLevel = 1;
    iopcon->EIRQ3.debounce = 1;

    return 0;
}