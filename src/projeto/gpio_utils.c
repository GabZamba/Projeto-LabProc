#include <stdio.h>
#include <stdint.h>
#include <string.h> // Incluir o cabeçalho para a função memset
#include "evlt7t.h"
#include "structs.h"

extern void delay(int);

/* Evaluator 7T */
// struct INTPND_t *intpnd = (struct INTPND_t *)0x03ff4004; // interrupções pendentes em 1
// struct IOPMOD_t *iopmod = (struct IOPMOD_t *)0x3ff5000;
// struct IOPDATA_t *iopdata = (struct IOPDATA_t *)0x3ff5008;
// struct INTMOD_t *intmod = (struct INTMOD_t *)0x03ff4000; // 1 se IRQ, 0 se FIQ (rápida)
// struct INTMSK_t *intmsk = (struct INTMSK_t *)0x03ff4008; // 1 se mascarado, 0 se habilitado
// struct TMOD_t *tmod = (struct TMOD_t *)0x03ff6000;       // configuração dos temporizadores
// int32_t *tdata0 = (int32_t *)0x03ff6004;                 // registrador de recarda do temp0
// int32_t *tcnt0 = (int32_t *)0x03ff600c;                 // registrador de recarda do temp0

/* QEMU */
struct INTPND_t *intpnd = (struct INTPND_t *)0x40100000; // interrupções pendentes em 1
struct IOPMOD_t *iopmod = (struct IOPMOD_t *)0x40100004;
struct IOPDATA_t *iopdata = (struct IOPDATA_t *)0x40100008;
struct INTMOD_t *intmod = (struct INTMOD_t *)0x4010000C; // 1 se IRQ, 0 se FIQ (rápida)
struct INTMSK_t *intmsk = (struct INTMSK_t *)0x40100010; // 1 se mascarado, 0 se habilitado
struct TMOD_t *tmod = (struct TMOD_t *)0x40100014;       // configuração dos temporizadores
int32_t *tdata0 = (int32_t *)0x40100018;                 // registrador de recarda do temp0
int32_t *tcnt0 = (int32_t *)0x4010001C;                  // registrador da contagem atual do temp0

const int32_t TIMER_VALUE = 49999999; // valor de recarga para 1s em 50 MHz

const int32_t DELAY_TIME = 1000000;

const int8_t DisplayNumber[16] = {0b1011111, 0b0000110, 0b0111011, 0b0101111, 0b1100110, 0b1101101, 0b1111101, 0b0000111, 0b1111111, 0b1101111, 0b1110111, 0b1111100, 0b1011001, 0b0111110, 0b1111001, 0b1110001};

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
    *tdata0 = TIMER_VALUE; // 1s

    // habilita a comtagem
    tmod->Timer0 = 0; // reinicia modo do timer 0
    tmod->Timer0_enable = 1;

    return 0;
}

uint32_t setLeds(uint32_t value)
{
    uint32_t prevValue = iopdata->leds;
    iopdata->leds = value;
    return prevValue;
}

uint32_t setDisplay(uint32_t value)
{
    uint32_t prevValue = iopdata->displays;
    iopdata->displays = value;
    return prevValue;
}

uint32_t setDisplayNumber(uint32_t num)
{
    uint32_t prevValue = iopdata->displays;
    iopdata->displays = DisplayNumber[num];
    return prevValue;
}

uint32_t getSwitches(void)
{
    return iopdata->switches;
}

void blinkNumber(uint32_t num)
{
    uint32_t prevValue = iopdata->displays;

    iopdata->displays = 0;
    delay(DELAY_TIME);
    iopdata->displays = DisplayNumber[num];
    delay(DELAY_TIME);
    iopdata->displays = 0;
    delay(DELAY_TIME);

    iopdata->displays = prevValue;

    return;
}

void blinkLeds(uint32_t value)
{
    uint32_t prevValue = iopdata->leds;

    iopdata->leds = 0;
    delay(DELAY_TIME);
    iopdata->leds = value;
    delay(DELAY_TIME);
    iopdata->leds = 0;
    delay(DELAY_TIME);

    iopdata->leds = prevValue;

    return;
}

void resetTimer(void)
{
    *tcnt0 = TIMER_VALUE;
}

void disableTimer(void)
{
    tmod->Timer0_enable = 0;
}

void enableTimer(void)
{
    tmod->Timer0_enable = 1;
}
