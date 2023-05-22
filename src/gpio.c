
#include "gpio.h"


enum Direction {
  DIR_INPUT,
  DIR_OUTPUT,
};

enum Input {
  INPUT_CONNECT = 0 << 1,
  INPUT_DISCONNECT = 1 << 1,
};

enum Pull {
  PULL_DISABLED = 0 << 8,
  PULL_PULLDOWN = 1 << 2,
  PULL_PULLUP = 3 << 2,
};

enum Drive {
  DRIVE_S0S1 = 0 << 8,
  DRIVE_H0S1 = 1 << 8,
  DRIVE_S0H1 = 2 << 8,
  DRIVE_H0H1 = 3 << 8,
  DRIVE_D0S1 = 4 << 8,
  DRIVE_D0H1 = 5 << 8,
  DRIVE_S0D1 = 6 << 8,
  DRIVE_H0D1 = 7 << 8,
};

enum Sense {
  SENSE_DISABLED = 0 << 16,
  SENSE_HIGH = 2 << 16,
  SENSE_LOW = 3 << 16,
};



static void Gpio_init(const Gpio* self){
  GPIO_CNF(self->port, self->pin) = 0 | DIR_INPUT | INPUT_DISCONNECT | PULL_DISABLED | DRIVE_S0S1 | SENSE_DISABLED;
  return;
}


Gpio Gpio_create(uint8_t port, uint8_t pin){
  Gpio gpio = {port, pin};
  Gpio_init(&gpio);
  return gpio;
}


void Gpio_config_connect(const Gpio* self){
  GPIO_CNF(self->port, self->pin) &= ~(INPUT_DISCONNECT);
  GPIO_CNF(self->port, self->pin) |= INPUT_CONNECT;
  return;
}


void Gpio_config_disconnect(const Gpio* self){
  GPIO_CNF(self->port, self->pin) &= ~(INPUT_DISCONNECT);
  GPIO_CNF(self->port, self->pin) |= INPUT_DISCONNECT;
  return;
}


void Gpio_config_output(const Gpio* self){
  Gpio_config_disconnect(self);
  GPIO_CNF(self->port, self->pin) &= ~(DIR_OUTPUT);
  GPIO_CNF(self->port, self->pin) |= DIR_OUTPUT;
  Gpio_set_low(self);
  Gpio_config_connect(self);
  return;
}


void Gpio_config_input(const Gpio* self){
  Gpio_config_disconnect(self);
  GPIO_CNF(self->port, self->pin) &= ~(DIR_OUTPUT);
  GPIO_CNF(self->port, self->pin) |= DIR_INPUT;
  Gpio_set_low(self);
  Gpio_config_connect(self);
  return;
}


void Gpio_set_high(const Gpio* self){
  Gpio_config_disconnect(self);
  Gpio_config_output(self);
  GPIO_OUT(self->port) |= (1 << self->pin);
  Gpio_config_connect(self);
  return;
}


void Gpio_set_low(const Gpio* self){
  Gpio_config_disconnect(self);
  Gpio_config_output(self);
  GPIO_OUT(self->port) &= ~(1 << self->pin);
  Gpio_config_connect(self);
  return;
}


uint8_t Gpio_read(const Gpio* self){
  uint8_t input = GPIO_IN(self->port) & (1 << self->pin);
  return input;
}
