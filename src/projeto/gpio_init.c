#include <stdio.h>
#include <stdint.h>
#include <string.h> // Incluir o cabeçalho para a função memset
#include "evlt7t.h"
#include "structs.h"

/* Evaluator 7T */
// struct INTPND_t *intpnd = (struct INTPND_t *)0x03ff4004; // interrupções pendentes em 1
// struct IOPMOD_t *iopmod = (struct IOPMOD_t *)0x3ff5000;
// struct IOPDATA_t *iopdata = (struct IOPDATA_t *)0x3ff5008;
// struct INTMOD_t *intmod = (struct INTMOD_t *)0x03ff4000; // 1 se IRQ, 0 se FIQ (rápida)
// struct INTMSK_t *intmsk = (struct INTMSK_t *)0x03ff4008; // 1 se mascarado, 0 se habilitado
// struct TMOD_t *tmod = (struct TMOD_t *)0x03ff6000;       // configuração dos temporizadores
// int32_t *tdata0 = (int32_t *)0x03ff6004;                 // registrador de recarda do temp1

/* QEMU */
struct INTPND_t *intpnd = (struct INTPND_t *)0x40100000; // interrupções pendentes em 1
struct IOPMOD_t *iopmod = (struct IOPMOD_t *)0x40100004;
struct IOPDATA_t *iopdata = (struct IOPDATA_t *)0x40100008;
struct INTMOD_t *intmod = (struct INTMOD_t *)0x4010000C; // 1 se IRQ, 0 se FIQ (rápida)
struct INTMSK_t *intmsk = (struct INTMSK_t *)0x40100010; // 1 se mascarado, 0 se habilitado
struct TMOD_t *tmod = (struct TMOD_t *)0x40100014;       // configuração dos temporizadores
int32_t *tdata0 = (int32_t *)0x40100018;                 // registrador de recarda do temp1

const int32_t TEMPO = 49999999; // valor de recarga para 1s em 50 MHz

int gpio_init()
{
    // define leds e display como saída
    iopmod->leds = 0b1111;
    iopmod->displays = 0b1111111;

    // configura timer0 como IRQ
    intmod->Timer0 = 0;

    // habilita interrupção glboal e do timer0
    intmsk->GlobalInterrupt = 0;
    intmsk->Timer0 = 0;

    // altera o valor de início do temporizador
    *tdata0 = 49999999; // 1s

    // habilita a comtagem
    tmod->Timer0 = 0; // reinicia modo do timer 0
    tmod->Timer0_enable = 1;

    return 0;
}