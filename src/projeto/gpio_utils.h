#ifndef GPIO_UTILS_H
#define GPIO_UTILS_H

#include <stdint.h>

extern void delay(int);

#define IRQ 0
#define FIQ 1
#define MASKED 1
#define ENABLED 0

#define TIMER_VALUE 50000000 // valor de recarga para 1s em 50 MHz

uint8_t getSwitches(void);

uint8_t getDisplay(void);

void clearDisplay(void);

void setDisplay(uint8_t value);

uint8_t getLeds(void);

void clearLeds(void);

void setLeds(uint8_t value);

/* 0 entrada, 1 saída */
void setLedsMode(uint8_t mode);

/* 0 entrada, 1 saída */
void setDisplayMode(uint8_t mode);

void enableGlobalInterrupt(void);

void disableGlobalInterrupt(void);

/* Displays the given 4 bit binary on the leds, and returns the previous value */
uint8_t setLedsValue(uint8_t value);

/* Displays the given 7 bit binary, and returns the previous value */
uint8_t setDisplayValue(uint8_t value);

/* Displays the given number, and returns the previous value */
uint8_t setDisplayNumber(uint8_t num);

void setTimer0Data(uint32_t value);

void resetTimer0Count(void);

void enableTimer0(void);

void disableTimer0(void);

void blinkNumber(uint8_t num);

void blinkLeds(uint32_t value);

void setTimer0Interruption(uint8_t mode);

/* 1 se mascarado, 0 se habilitado */
void setTimer0InterruptionMask(uint8_t mode);

void gpio_init();

#endif
