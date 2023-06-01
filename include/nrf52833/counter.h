
#ifndef COUNTER_H
#define COUNTER_H

#include <stdint.h>

typedef struct {
  uint8_t const unit;
  uint32_t const periodMicroSec;
} Counter;


Counter counter_create(uint8_t const unit, uint32_t const period);
void counter_reset(Counter const* const self);

void counter_start(Counter const* const self);
void counter_stop(Counter const* const self);
void counter_count(Counter const* const self, uint32_t countTo);

void sleep(uint32_t milliSeconds);

#endif
