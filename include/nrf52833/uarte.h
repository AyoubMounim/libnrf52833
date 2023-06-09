
#ifndef UARTE_H
#define UARTE_H

#include "gpio.h"
#include <stdint.h>


typedef enum {
  BAUD_9600 = 0x00275000,
  BAUD_115200 = 0x01D60000,
  BAUD_250000 = 0x04000000
} Baudrate;

typedef enum {
  STOPBITS_ONE,
  STOPBITS_TWO
} Stopbits;


typedef struct {
  uint8_t const unit;
  uint8_t maxIputLen;
} Uarte;


Uarte uarte_create(uint8_t const unit, uint8_t const maxInputLen);
void uarte_init(Uarte const* const self);
void uarte_reset(Uarte const* const self);
void uarte_enable(Uarte const* const self);
void uarte_disable(Uarte const* const self);
uint8_t uarte_isEnabled(Uarte const* const self);

void uarte_setTxPin(Uarte const* const self, Gpio const* const pin);
void uarte_setRxPin(Uarte const* const self, Gpio const* const pin);
void uarte_setCtsPin(Uarte const* const self, Gpio const* const pin);
void uarte_setRtsPin(Uarte const* const self, Gpio const* const pin);

void uarte_setBaudrate(Uarte const* const self, Baudrate baudrate);
Baudrate uarte_getBaudrate(Uarte const* const self);
void uarte_setStopbits(Uarte const* const self, Stopbits stopbits);
void uarte_setParityNone(Uarte const* const self);
void uarte_setParityEven(Uarte const* const self);
void uarte_setParityOdd(Uarte const* const self);
void uarte_enableHwfc(Uarte const* const self);
void uarte_disableHwfc(Uarte const* const self);
void uarte_enableEndRxToStartRxShort(Uarte const* const self);
void uarte_disableEndRxToStartRxShort(Uarte const* const self);
void uarte_enableEndRxToStopRxShort(Uarte const* const self);
void uarte_disableEndRxToStopRxShort(Uarte const* const self);

void uarte_writeChar(Uarte const* const self, char const* const ch);
void uarte_writeStr(Uarte const* const self, char const* const str);
void uarte_writeInt(Uarte const* const self, int32_t integer);
void uarte_getChar(Uarte const* const self, char* ch_input);
void uarte_inputStr(Uarte const* const self, char* strInput);
uint32_t uarte_inputInt(Uarte const* const self);
void uarte_flush(Uarte const* const self);


#endif
