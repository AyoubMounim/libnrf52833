
#ifndef I2C_H
#define I2C_H

#include "nrf52833/gpio.h"
#include <stdint.h>

typedef enum {
  UNIT_0 = 0x0,
  UNIT_1 = 0x1
} Unit;

typedef struct {
  Unit const unit;
} Twim;

typedef enum {
  K100 = 0x01980000,
  K250 = 0x04000000,
  K400 = 0x06400000
} TwimFreq;


Twim twim_create(Unit unit, Gpio const* const sclPin, Gpio const* const sdaPin);
void twim_reset(Twim const* const self);

void twim_enable(Twim const* const self);
void twim_disable(Twim const* const self);
void twim_resume(Twim const* const self);
void twim_stop(Twim const* const self);

uint8_t twim_isStarted(Twim const* const self);
uint8_t twim_isEnabled(Twim const* const self);
uint8_t twim_isSuspended(Twim const* const self);

void twim_setSclPin(Twim const* const self, Gpio const* const pin);
void twim_setSdaPin(Twim const* const self, Gpio const* const pin);
void twim_setFrequency(Twim const* const self, TwimFreq freq);
void twim_setAddress(Twim const* const self, uint32_t addr);

void twim_write(Twim const* const self, void* dataPtr, uint32_t dataLen, uint32_t slaveAddr);
void twim_read(Twim const* const self, void* bufferPtr, uint32_t bufferLen, uint32_t slaveAddr);

void twim_shortLastTxToStartRxEnable(Twim const* const self);
void twim_shortLastTxToStartRxDisable(Twim const* const self);
void twim_shortLastTxToSuspendEnable(Twim const* const self);
void twim_shortLastTxToSuspendDisable(Twim const* const self);
void twim_shortLastTxToStopEnable(Twim const* const self);
void twim_shortLastTxToStopDisable(Twim const* const self);
void twim_shortLastRxToStartTxEnable(Twim const* const self);
void twim_shortLastRxToStartTxDisable(Twim const* const self);
void twim_shortLastRxToSuspendEnable(Twim const* const self);
void twim_shortLastRxToSuspendDisable(Twim const* const self);
void twim_shortLastRxToStopEnable(Twim const* const self);
void twim_shortLastRxToStopDisable(Twim const* const self);


#endif
