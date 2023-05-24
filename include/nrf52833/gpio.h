
#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>


typedef struct {
  uint8_t const port;
  uint8_t const pin;
} Gpio;

/* constructors */
Gpio gpio_create(uint8_t const port, uint8_t const pin);

/* pin configuration functions */
void gpio_configConnect(Gpio const* const self);
void gpio_configDisconnect(Gpio const* const self);
void gpio_configOutput(Gpio const* const self);
void gpio_configInput(Gpio const* const self);

/* pin action functions */
void gpio_setHigh(Gpio const* const self);
void gpio_setLow(Gpio const* const self);
uint8_t gpio_read(Gpio const* const self);


#endif

