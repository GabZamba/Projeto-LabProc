#ifndef GPIO_UTILS_H
#define GPIO_UTILS_H

#include <stdint.h>

extern void delay(int);

int gpio_init(void);

uint32_t setLeds(uint32_t value);

uint32_t setDisplay(uint32_t value);

uint32_t setDisplayNumber(uint32_t num);

uint32_t getSwitches(void);

void blinkNumber(uint32_t num);

void blinkLeds(uint32_t num);

void resetTimer(void);

void disableTimer(void);

void enableTimer(void);

#endif
