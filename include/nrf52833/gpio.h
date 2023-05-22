
#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>
#include "macros.h"


#define GPIO_BASE 0x50000000

#define GPIO_CNF(port, pin) REG(uint32_t, GPIO_BASE, (0x300*port + 0x700 + 0x4*pin))
#define GPIO_IN(port) REG(uint32_t, GPIO_BASE, (0x300*port + 0x510)) 
#define GPIO_OUT(port) REG(uint32_t, GPIO_BASE, (0x300*port + 0x504)) 


typedef struct Gpio Gpio;
struct Gpio {
  const uint8_t port;
  const uint8_t pin;
};

/* constructors */
Gpio Gpio_create(uint8_t port, uint8_t pin);

/* pin configuration functions */
void Gpio_configConnect(const Gpio* self);
void Gpio_configDisconnect(const Gpio* self);
void Gpio_configOutput(const Gpio* self);
void Gpio_configInput(const Gpio* self);

/* pin action functions */
void Gpio_setHigh(const Gpio* self);
void Gpio_setLow(const Gpio* self);
uint8_t Gpio_read(const Gpio* self);


#endif
