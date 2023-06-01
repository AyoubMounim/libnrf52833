
#ifndef PWM_H
#define PWM_H

#include "nrf52833/gpio.h"
#include <stdint.h>


typedef struct {
  uint8_t const unit;
} Pwm;


Pwm pwm_create(uint8_t const unit);
void pwm_reset(Pwm const* const self);


#endif
