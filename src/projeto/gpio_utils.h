#ifndef GPIO_UTILS_H
#define GPIO_UTILS_H

#include <stdint.h>

extern void delay(int);

#define IRQ 0
#define FIQ 1
#define MASKED 1
#define ENABLED 0

// 1 second on 50 MHz
#define ONE_SECOND 50000000

#define DELAY_TIME 10000000;

/**
 *  GPIO
 */

uint8_t getSwitches(void);

/* 0 input, 1 output */
void setDisplayMode(uint8_t mode);

void clearDisplay(void);

uint8_t getDisplay(void);

void setDisplay(uint8_t value);

/* Displays the given 7 bit binary, and returns the previous value */
uint8_t changeDisplayValue(uint8_t value);

/* Displays the given number, and returns the previous value */
uint8_t changeDisplayNumber(uint8_t num);

/* 0 input, 1 output */
void setLedsMode(uint8_t mode);

void clearLeds(void);

void setLeds(uint8_t value);

uint8_t getLeds(void);

/* Displays the given 4 bit binary on the leds, and returns the previous value */
uint8_t changeLedsValue(uint8_t value);

void gpio_init();

/**
 *  INTERRUPTIONS
 */

void enableGlobalInterrupt(void);

void disableGlobalInterrupt(void);

void setTimer0Interruption(uint8_t mode);

void setTimer0InterruptionMask(uint8_t mode); /* 0 enabled, 1 masked */

/**
 *  TIMER
 */

void setTimer0Data(uint32_t value);

void resetTimer0Count(void);

uint32_t getTimer0Count(void);

uint32_t getTimer0Data(void);

void enableTimer0(void);

void disableTimer0(void);

#endif
