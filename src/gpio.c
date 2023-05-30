
#include "nrf52833/gpio.h"
#include "macros.h"


#define GPIO_BASE 0x50000000

#define GPIO_CNF(port, pin) REG(uint32_t, GPIO_BASE, (0x300*port + 0x700 + 0x4*pin))
#define GPIO_IN(port) REG(uint32_t, GPIO_BASE, (0x300*port + 0x510)) 
#define GPIO_OUT(port) REG(uint32_t, GPIO_BASE, (0x300*port + 0x504)) 


enum Direction {
  DIR_INPUT = 0,
  DIR_OUTPUT = 1,
  DIR_POS = 0,
  DIR_WIDTH = 1
};

enum Input {
  INPUT_CONNECT = 0,
  INPUT_DISCONNECT = 1,
  INPUT_POS = 1,
  INPUT_WIDTH = 1
};

enum Pull {
  PULL_DISABLED = 0,
  PULL_PULLDOWN = 1,
  PULL_PULLUP = 3,
  PULL_POS = 2,
  PULL_WIDTH = 2
};

enum {
  DRIVE_S0S1 = 0,
  DRIVE_H0S1 = 1,
  DRIVE_S0H1 = 2,
  DRIVE_H0H1 = 3,
  DRIVE_D0S1 = 4,
  DRIVE_D0H1 = 5,
  DRIVE_S0D1 = 6,
  DRIVE_H0D1 = 7,
  DRIVE_POS = 8,
  DRIVE_WIDTH = 3
};

enum Sense {
  SENSE_DISABLED = 0,
  SENSE_HIGH = 2,
  SENSE_LOW = 3,
  SENSE_POS = 16,
  SENSE_WIDTH = 2
};


static void gpio_init(Gpio const* const self){
  GPIO_CNF(self->port, self->pin) = 0;
  SET_FIELD(GPIO_CNF(self->port, self->pin), DIR_POS, DIR_WIDTH, DIR_INPUT);
  SET_FIELD(GPIO_CNF(self->port, self->pin), INPUT_POS, INPUT_WIDTH, INPUT_DISCONNECT);
  SET_FIELD(GPIO_CNF(self->port, self->pin), PULL_POS, PULL_WIDTH, PULL_DISABLED);
  SET_FIELD(GPIO_CNF(self->port, self->pin), DRIVE_POS, DRIVE_WIDTH, DRIVE_S0S1);
  SET_FIELD(GPIO_CNF(self->port, self->pin), SENSE_POS, SENSE_WIDTH, SENSE_DISABLED);
  return;
}


Gpio gpio_create(uint8_t const port, uint8_t const pin){
  Gpio gpio = {port, pin};
  gpio_init(&gpio);
  return gpio;
}


void gpio_configConnect(Gpio const* const self){
  SET_FIELD(GPIO_CNF(self->port, self->pin), INPUT_POS, INPUT_WIDTH, INPUT_CONNECT);
  return;
}


void gpio_configDisconnect(Gpio const* const self){
  SET_FIELD(GPIO_CNF(self->port, self->pin), INPUT_POS, INPUT_WIDTH, INPUT_DISCONNECT);
  return;
}


void gpio_configOutput(Gpio const* const self){
  SET_FIELD(GPIO_CNF(self->port, self->pin), DIR_POS, DIR_WIDTH, DIR_OUTPUT);
  return;
}


void gpio_configInput(Gpio const* const self){
  SET_FIELD(GPIO_CNF(self->port, self->pin), DIR_POS, DIR_WIDTH, DIR_INPUT);
  return;
}

void gpio_setDriveS0D1(Gpio const* const self){
  SET_FIELD(
    GPIO_CNF(self->port, self->pin),
    DRIVE_POS,
    DRIVE_WIDTH,
    DRIVE_S0D1
  );
  return;
}


void gpio_setHigh(Gpio const* const self){
  SET_BIT(GPIO_OUT(self->port), self->pin);
  return;
}


void gpio_setLow(Gpio const* const self){
  CLR_BIT(GPIO_OUT(self->port), self->pin);
  return;
}


uint8_t gpio_read(Gpio const* const self){
  uint8_t input = GET_BIT(GPIO_IN(self->port), self->pin);
  return input;
}

