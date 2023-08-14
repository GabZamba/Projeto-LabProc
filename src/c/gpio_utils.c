#include <stdio.h>
#include <stdint.h>
#include <string.h> // Includes the header of the memset function
#include "evlt7t.h"
#include "gpio_utils.h"

extern void delay(int);

/* Evaluator 7T */
uint32_t *intmod = INTMOD; // 1 IRQ, 0 FIQ (fast)
uint32_t *intpnd = INTPND; // pending interruptions in 1
uint32_t *intmsk = INTMSK; // 1 masked, 0 enabled

uint32_t *iopmod = IOPMOD;   // GPIO configuration
uint32_t *iopdata = IOPDATA; // GPIO values

uint32_t *tmod = TMOD;     // timer configuration
uint32_t *tdata0 = TDATA0; // recharge register of timer 0
uint32_t *tcnt0 = TCNT0;   // count register of timer 0

/* QEMU */
// uint32_t *intmod = (uint32_t *)0x4010000C; // 1 IRQ, 0 FIQ (fast)
// uint32_t *intpnd = (uint32_t *)0x40100000; // pending interruptions in 1
// uint32_t *intmsk = (uint32_t *)0x40100010; // 1 masked, 0 enabled

// uint32_t *iopmod = (uint32_t *)0x40100004;  // GPIO mode (0 input, 0 output)
// uint32_t *iopdata = (uint32_t *)0x40100008; // GPIO values

// uint32_t *tmod = (uint32_t *)0x40100014;   // timer configuration
// uint32_t *tdata0 = (uint32_t *)0x40100018; // recharge register of timer 0
// uint32_t *tcnt0 = (uint32_t *)0x4010001C;  // count register of timer 0

// conversion fom hexadecimal to 7 segments value
const int8_t HexaTo7Seg[16] = {0b1011111, 0b0000110, 0b0111011, 0b0101111, 0b1100110, 0b1101101, 0b1111101, 0b0000111, 0b1111111, 0b1101111, 0b1110111, 0b1111100, 0b1011001, 0b0111110, 0b1111001, 0b1110001};

/**
 * GPIO
 */

uint8_t getSwitches(void) { return *iopdata & 0xf; }

/* 0 input, 1 output */
void setDisplayMode(uint8_t mode) { *iopmod |= (mode & 0x7f) << 10; }

void clearDisplay(void) { *iopdata &= ~(0x7f << 10); }

void setDisplay(uint8_t value) { *iopdata |= (value & 0x7f) << 10; }

uint8_t getDisplay(void) { return (*iopdata & (0x7f << 10)) >> 10; }

/* Displays the given 7 bit binary, and returns the previous value */
uint8_t changeDisplayValue(uint8_t value)
{
    uint8_t prevValue = getDisplay();
    clearDisplay();
    setDisplay(value);
    return prevValue;
}

/* Displays the given number, and returns the previous value */
uint8_t changeDisplayNumber(uint8_t num)
{
    uint8_t prevValue = getDisplay();
    clearDisplay();
    setDisplay(HexaTo7Seg[num & 0xf]);
    return prevValue;
}

/* 0 input, 1 output */
void setLedsMode(uint8_t mode) { *iopmod |= (mode & 0xf) << 4; }

void clearLeds(void) { *iopdata &= ~(0xf << 4); }

void setLeds(uint8_t value) { *iopdata |= (value & 0xf) << 4; }

uint8_t getLeds(void) { return (*iopdata & 0xf0) >> 4; }

/* Displays the given 4 bit binary on the leds, and returns the previous value */
uint8_t changeLedsValue(uint8_t value)
{
    uint8_t prevValue = getLeds();
    clearLeds();
    setLeds(value);
    return prevValue;
}

void gpio_init()
{
    // define leds and display as output
    setLedsMode(0b1111);
    setDisplayMode(0b1111111);

    clearDisplay();
    clearLeds();

    setTimer0Interruption(IRQ);

    enableGlobalInterrupt();
    setTimer0InterruptionMask(ENABLED);

    setTimer0Data(ONE_SECOND);
    enableTimer0();
}

/**
 *  INTERRUPTIONS
 */

void enableGlobalInterrupt(void) { *intmsk &= ~(0b1 << 21); }

void disableGlobalInterrupt(void) { *intmsk |= (0b1 << 21); }

/* 0 IRQ, 1 FIQ (fast) */
void setTimer0Interruption(uint8_t mode)
{
    if (mode == IRQ)
        bit_clr(*intmod, 10);
    else
        bit_set(*intmod, 10);
}

/* 1 masked, 0 enabled */
void setTimer0InterruptionMask(uint8_t mode)
{
    if (mode == ENABLED)
        bit_clr(*intmsk, 10);
    else
        bit_set(*intmsk, 10);
}

/**
 *  TIMER
 */

void setTimer0Data(uint32_t value) { *tdata0 = value; }

void resetTimer0Count(void) { *tcnt0 = *tdata0; }

uint32_t getTimer0Count(void) { return *tcnt0; };

uint32_t getTimer0Data(void) { return *tdata0; };

void enableTimer0(void) { *tmod |= (0b1); }

void disableTimer0(void) { *tmod &= (0b0); }
