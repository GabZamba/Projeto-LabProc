#include <stdio.h>
#include <stdint.h>
#include <string.h> // Incluir o cabeçalho para a função memset
#include "evlt7t.h"
#include "structs.h"

extern void delay(int);

/* Evaluator 7T */
// uint32_t *intpnd = (uint32_t *)0x03ff4004; // interrupções pendentes em 1
// uint32_t *iopmod = (uint32_t *)0x3ff5000;
// uint32_t *iopdata = (uint32_t *)0x3ff5008;
// uint32_t *intmod = (uint32_t *)0x03ff4000; // 1 se IRQ, 0 se FIQ (rápida)
// uint32_t *intmsk = (uint32_t *)0x03ff4008; // 1 se mascarado, 0 se habilitado
// uint32_t *tmod = (uint32_t *)0x03ff6000;   // configuração dos temporizadores
// uint32_t *tdata0 = (uint32_t *)0x03ff6004; // registrador de recarda do temp0
// uint32_t *tcnt0 = (uint32_t *)0x03ff600c;  // registrador de recarda do temp0

/* QEMU */
uint32_t *intpnd = (uint32_t *)0x40100000; // interrupções pendentes em 1
uint32_t *iopmod = (uint32_t *)0x40100004;
uint32_t *iopdata = (uint32_t *)0x40100008;
uint32_t *intmod = (uint32_t *)0x4010000C; // 1 se IRQ, 0 se FIQ (rápida)
uint32_t *intmsk = (uint32_t *)0x40100010; // 1 se mascarado, 0 se habilitado
uint32_t *tmod = (uint32_t *)0x40100014;   // configuração dos temporizadores
uint32_t *tdata0 = (uint32_t *)0x40100018; // registrador de recarda do temp0
uint32_t *tcnt0 = (uint32_t *)0x4010001C;  // registrador da contagem atual do temp0

#define IRQ 1
#define FIQ 0
#define MASKED 1
#define ENABLED 0

const int32_t TIMER_VALUE = 49999999; // valor de recarga para 1s em 50 MHz

const int32_t DELAY_TIME = 1000000;

const int8_t DisplayNumber[16] = {0b1011111, 0b0000110, 0b0111011, 0b0101111, 0b1100110, 0b1101101, 0b1111101, 0b0000111, 0b1111111, 0b1101111, 0b1110111, 0b1111100, 0b1011001, 0b0111110, 0b1111001, 0b1110001};

/* Funções Auxiliares */

uint8_t getSwitches(void) { return *iopdata & 0xf; }

uint8_t getDisplay(void) { return (*iopdata & (0x7f << 10)) >> 10; }

void clearDisplay(void) { *iopdata &= ~(0x7f << 10); }

void setDisplay(uint8_t value) { *iopdata |= (value & 0x7f) << 10; }

uint8_t getLeds(void) { return (*iopdata & 0xf0) >> 4; }

void clearLeds(void) { *iopdata &= ~(0xf << 4); }

void setLeds(uint8_t value) { *iopdata |= (value & 0xf) << 4; }

/* 0 entrada, 1 saída */
void setLedsMode(uint8_t mode) { *iopmod |= (mode & 0xf) << 4; }

/* 0 entrada, 1 saída */
void setDisplayMode(uint8_t mode) { *iopmod |= (mode & 0x7f) << 10; }

void enableGlobalInterrupt(void) { *intmsk &= (0b0 << 21); }

void disableGlobalInterrupt(void) { *intmsk |= (0b1 << 21); }

/* Displays the given 4 bit binary on the leds, and returns the previous value */
uint8_t setLedsValue(uint8_t value)
{
    uint8_t prevValue = getLedsValue();
    clearLeds();
    setLedsValue(value);
    return prevValue;
}

/* Displays the given 7 bit binary, and returns the previous value */
uint8_t setDisplayValue(uint8_t value)
{
    uint8_t prevValue = getDisplayValue();
    clearDisplay();
    setDisplayValue(value);
    return prevValue;
}

/* Displays the given number, and returns the previous value */
uint8_t setDisplayNumber(uint8_t num)
{
    uint8_t prevValue = getDisplayValue();
    clearDisplay();
    setDisplayValue(DisplayNumber[num & 0x1f]);
    return prevValue;
}

void setTimer0Data(uint32_t value) { *tdata0 = value; }

void resetTimer0Count(void) { *tcnt0 = *tdata0; }

void enableTimer0(void) { *tmod |= (0b1); }

void disableTimer0(void) { *tmod &= (0b0); }

void blinkNumber(uint8_t num)
{
    uint8_t prevValue = getDisplayValue();

    clearDisplay();
    delay(DELAY_TIME);

    setDisplayValue(DisplayNumber[num & 0x1f]);
    delay(DELAY_TIME);

    clearDisplay();
    delay(DELAY_TIME);

    setDisplayValue(prevValue);

    return;
}

void blinkLeds(uint32_t value)
{
    uint8_t prevValue = getLedsValue();

    clearLeds();
    delay(DELAY_TIME);

    setLedsValue(value);
    delay(DELAY_TIME);

    clearLeds();
    delay(DELAY_TIME);

    setLedsValue(prevValue);

    return;
}

/* 1 se IRQ, 0 se FIQ (rápida) */
void setTimer0Interruption(uint8_t mode)
{
    if (mode == FIQ)
        bit_clr(*intmod, 10);
    else
        bit_set(*intmod, 10);
}

/* 1 se mascarado, 0 se habilitado */
void setTimer0InterruptionMask(uint8_t mode)
{
    if (mode == ENABLED)
        bit_clr(*intmod, 10);
    else
        bit_set(*intmod, 10);
}

void gpio_init()
{
    // // define leds e display como saída
    setLedsMode(0b1111);
    setDisplayMode(0b1111111);

    setTimerInterruption(IRQ);

    enableGlobalInterrupt();
    setTimerInterruptionMask(ENABLED);

    setTimer0Data(TIMER_VALUE); // 1s
    enableTimer0();
}