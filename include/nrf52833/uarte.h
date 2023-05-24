
#ifndef UARTE_H
#define UARTE_H

#include "gpio.h"


typedef struct Uarte Uarte;
struct Uarte {
  const uint8_t maxIputLen;
  Gpio txPin;
  Gpio rxPin;
};


Uarte Uarte_create(Gpio* txPin, Gpio* rxPin);

void Uarte_init(Uarte* self);
void Uarte_enable(Uarte* self);
void Uarte_disable(Uarte* self);
void Uarte_setTxPin(Uarte* self);
void Uarte_setRxPin(Uarte* self);

void Uarte_writeChar(Uarte* self, const char* ch);
void Uarte_writeStr(Uarte* self, const char* str);
void Uarte_writeInt(Uarte* self, uint32_t integer, uint8_t base);

void Uarte_input(Uarte* self, char* input);
void Uarte_getChar(Uarte* self, char* ch);
void Uarte_listenChar(Uarte* self, char* ch);
void Uarte_endl(Uarte* self);
void Uarte_flush(Uarte* self);

void Uarte_setBaudrate9600(Uarte* self);
void Uarte_setBaudrate115200(Uarte* self);
void Uarte_setParityNone(Uarte* self);
void Uarte_setParityEven(Uarte* self);
void Uarte_setParityOdd(Uarte* self);
void Uarte_setHwfcEnable(Uarte* self);
void Uarte_setHwfcDisable(Uarte* self);


#endif
