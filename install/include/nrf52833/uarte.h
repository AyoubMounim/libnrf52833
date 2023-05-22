
#ifndef UARTE_H
#define UARTE_H

#include <stdint.h>
#include "macros.h"
#include "gpio.h"


#define UARTE0_BASE 0x40002000

/* tasks */
#define UARTE0_STARTRX REG(uint32_t, UARTE0_BASE, 0x000)
#define UARTE0_STOPRX  REG(uint32_t, UARTE0_BASE, 0x004)
#define UARTE0_STARTTX REG(uint32_t, UARTE0_BASE, 0x008)
#define UARTE0_STOPTX REG(uint32_t, UARTE0_BASE, 0x00C)
#define UARTE0_FLUSHRX REG(uint32_t, UARTE0_BASE, 0x02C)

/* events */
#define UARTE0_CTS REG(uint32_t, UARTE0_BASE, 0x100)
#define UARTE0_NTCS REG(uint32_t, UARTE0_BASE, 0x104)
#define UARTE0_RXDRDY REG(uint32_t, UARTE0_BASE, 0x108)
#define UARTE0_ENDRX REG(uint32_t, UARTE0_BASE, 0x110)
#define UARTE0_TXDRDY REG(uint32_t, UARTE0_BASE, 0x11C)
#define UARTE0_ENDTX REG(uint32_t, UARTE0_BASE, 0x120)
#define UARTE0_ERROR REG(uint32_t, UARTE0_BASE, 0x124)
#define UARTE0_RXTO REG(uint32_t, UARTE0_BASE, 0x144)
#define UARTE0_RXSTARTED REG(uint32_t, UARTE0_BASE, 0x14C)
#define UARTE0_TXSTARTED REG(uint32_t, UARTE0_BASE, 0x150)
#define UARTE0_TXSTOPPED REG(uint32_t, UARTE0_BASE, 0x158)
#define UARTE0_SHORTS REG(uint32_t, UARTE0_BASE, 0x200)

/* interrupts */
#define UARTE0_INTEN REG(uint32_t, UARTE0_BASE, 0x300)
#define UARTE0_INTENSET REG(uint32_t, UARTE0_BASE, 0x304)
#define UARTE0_INTENCLR REG(uint32_t, UARTE0_BASE, 0x308)
#define UARTE0_ERRORSRC REG(uint32_t, UARTE0_BASE, 0x480)

/* registers */
#define UARTE0_ENABLE REG(uint32_t, UARTE0_BASE, 0x500)
#define UARTE0_PSELRTS REG(uint32_t, UARTE0_BASE, 0x508)
#define UARTE0_PSELTXD REG(uint32_t, UARTE0_BASE, 0x50C)
#define UARTE0_PSELCTS REG(uint32_t, UARTE0_BASE, 0x510)
#define UARTE0_PSELRXD REG(uint32_t, UARTE0_BASE, 0x514)
#define UARTE0_BAUDRATE REG(uint32_t, UARTE0_BASE, 0x524)
#define UARTE0_RXD REG(uint32_t, UARTE0_BASE, 0x534)
#define UARTE0_RXDMAXCNT REG(uint32_t, UARTE0_BASE, 0x538)
#define UARTE0_RXDAMOUNNT REG(uint32_t, UARTE0_BASE, 0x53C)
#define UARTE0_TXD REG(uint32_t, UARTE0_BASE, 0x544)
#define UARTE0_TXDMAXCNT REG(uint32_t, UARTE0_BASE, 0x548)
#define UARTE0_TXDAMOUNNT REG(uint32_t, UARTE0_BASE, 0x54C)
#define UARTE0_CONFIG REG(uint32_t, UARTE0_BASE, 0x56C)

#define MAX_INPUT_LEN 32


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

void Uarte_writeChar(const char* ch);
void Uarte_writeStr(const char* str);
void Uarte_writeInt(uint32_t integer, uint8_t base);

void Uarte_input(char* input);
void Uarte_getChar(char* ch);
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
