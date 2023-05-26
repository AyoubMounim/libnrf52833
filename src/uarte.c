
#include <stdint.h>
#include "gpio.h"
#include "uarte.h"
#include "macros.h"
#include "utils.h"


#define UARTE_BASE(unit) (0x40002000 + unit*0x26000)

#define UARTE_SHORTS(unit) REG(uint32_t, UARTE_BASE(unit), 0x200)
#define UARTE_ERRORSRC(unit) REG(uint32_t, UARTE_BASE(unit), 0x480)
#define UARTE_ENABLE(unit) REG(uint32_t, UARTE_BASE(unit), 0x500)
#define UARTE_BAUDRATE(unit) REG(uint32_t, UARTE_BASE(unit), 0x524)
#define UARTE_CONFIG(unit) REG(uint32_t, UARTE_BASE(unit), 0x56C)

#define MAX_INPUT_LEN 32


enum {
  UARTE_SHORTS_ENDRX_STARTRX_POS = 5,
  UARTE_SHORTS_ENDRX_STOPRX_POS = 6
};

enum {
  UARTE_ERRORSRC_OVERRUN_POS = 0,
  UARTE_ERRORSRC_PARITY_POS = 1,
  UARTE_ERRORSRC_FRAMING_POS = 2,
  UARTE_ERRORSRC_BREAK_POS = 3
};

enum {
  UARTE_ENABLE_DISABLED = 0,
  UARTE_ENABLE_ENABLED = 8,
  UARTE_ENABLE_WIDTH = 4,
  UARTE_ENABLE_POS = 0
};

enum {
  UARTE_BAUDRATE_9600 = 0x00275000,
  UARTE_BAUDRATE_115200 = 0x01D60000,
};

enum {
  UARTE_CONFIG_PARITY_EXCLUDED = 0,
  UARTE_CONFIG_PARITY_INCLUDED = 7,
  UARTE_CONFIG_PARITY_WIDTH = 3,
  UARTE_CONFIG_PARITY_POS = 1,
  UARTE_CONFIG_HWFC_POS = 0,
  UARTE_CONFIG_STOP_POS = 4,
  UARTE_CONFIG_PARITYTYPE_POS = 8
};


typedef struct {
  uint32_t startRx;
  uint32_t stopRx;
  uint32_t startTx;
  uint32_t stopTx;
  uint64_t _reserved0;
  uint64_t _reserved1;
  uint64_t _reserved2;
  uint32_t _reserved3;
  uint32_t flushRx;
} UarteTask;


typedef struct {
  uint32_t cts;
  uint32_t ncts;
  uint32_t rxdRdy;
  uint32_t _reserved0;
  uint32_t endRx;
  uint64_t _reserved1;
  uint32_t txdRdy;
  uint32_t endTx;
  uint32_t error;
  uint64_t _reserved2;
  uint64_t _reserved3;
  uint64_t _reserved4;
  uint32_t _reserved5;
  uint32_t rxTo;
  uint32_t _reserved6;
  uint32_t rxStarted;
  uint32_t txStarted;
  uint32_t _reserved7;
  uint32_t txStopped;
} UarteEvent;


typedef struct {
  uint32_t inten;
  uint32_t intenSet;
  uint32_t intenClr;
} UarteInterrupt;


typedef struct {
  uint32_t pselRts;
  uint32_t pselTxd;
  uint32_t pselCts;
  uint32_t pselRxd;
} UartePin;


typedef struct {
  uint32_t rxdPtr;
  uint32_t rxdMaxCnt;
  uint32_t rxdAmount;
  uint32_t _reserved0;
  uint32_t txdPtr;
  uint32_t txdMaxCnt;
  uint32_t txdAmount;
} UarteData;


UarteTask* pUarteTask[] = {
  (UarteTask*) UARTE_BASE(0),
  (UarteTask*) UARTE_BASE(1)
};
UarteEvent* pUarteEvent[] = {
  (UarteEvent*) (UARTE_BASE(0) + 0x100),
  (UarteEvent*) (UARTE_BASE(1) + 0x100)
};
UarteInterrupt* pUarteInterrupt[] = {
  (UarteInterrupt*) (UARTE_BASE(0) + 0x300),
  (UarteInterrupt*) (UARTE_BASE(1) + 0x300)
};
UartePin* pUartePin[] = {
  (UartePin*) (UARTE_BASE(0) + 0x508),
  (UartePin*) (UARTE_BASE(1) + 0x508)
};
UarteData* pUarteData[] = {
  (UarteData*) (UARTE_BASE(0) + 0x534),
  (UarteData*) (UARTE_BASE(1) + 0x534)
};


void uarte_init(Uarte const* const self);
void uarte_enable(Uarte const* const self);
void uarte_disable(Uarte const* const self);
void uarte_includeParity(Uarte const* const self);
void uarte_excludeParity(Uarte const* const self);
void uarte_eventsReset(Uarte const* const self);


Uarte uarte_create(uint8_t const unit, uint8_t const maxInputLen){
  Uarte uarte = {unit, maxInputLen};
  uarte_init(&uarte);
  return uarte;
}

void uarte_setTxPin(Uarte const* const self, Gpio const* const pin){
  uarte_disable(self);
  gpio_configDisconnect(pin);
  gpio_configOutput(pin);
  gpio_setHigh(pin);
  gpio_configConnect(pin);
  pUartePin[self->unit]->pselTxd = (pin->port << 5) + pin->pin;
  uarte_enable(self);
}

void uarte_setRxPin(Uarte const* const self, Gpio const* const pin){
  uarte_disable(self);
  gpio_configDisconnect(pin);
  gpio_configInput(pin);
  gpio_configConnect(pin);
  pUartePin[self->unit]->pselRxd = (pin->port << 5) + pin->pin;
  uarte_enable(self);
}

void uarte_setCtsPin(Uarte const* const self, Gpio const* const pin){
  uarte_disable(self);
  gpio_configDisconnect(pin);
  gpio_configInput(pin);
  gpio_configConnect(pin);
  pUartePin[self->unit]->pselCts = (pin->port << 5) + pin->pin;
  uarte_enable(self);
}

void uarte_setRtsPin(Uarte const* const self, Gpio const* const pin){
  uarte_disable(self);
  gpio_configDisconnect(pin);
  gpio_configOutput(pin);
  gpio_setHigh(pin);
  gpio_configConnect(pin);
  pUartePin[self->unit]->pselRts = (pin->port << 5) + pin->pin;
  uarte_enable(self);
}

void uarte_setBaudrate(Uarte const* const self, Baudrate baudrate){
  UARTE_BAUDRATE(self->unit) = baudrate;
  return;
}

void uarte_setStopbits(Uarte const* const self, Stopbits stopbits){
  switch (stopbits){
    case STOPBITS_ONE:
      CLR_BIT(UARTE_CONFIG(self->unit), UARTE_CONFIG_STOP_POS);
      break;  
    case STOPBITS_TWO:
      SET_BIT(UARTE_CONFIG(self->unit), UARTE_CONFIG_STOP_POS);
      break;  
  }
  return;
}

void uarte_setParityNone(Uarte const* const self){
  uarte_excludeParity(self);
  return;
}

void uarte_setParityEven(Uarte const* const self){
  uarte_includeParity(self);
  CLR_BIT(UARTE_CONFIG(self->unit), UARTE_CONFIG_PARITYTYPE_POS);
  return;
}

void uarte_setParityOdd(Uarte const* const self){
  uarte_includeParity(self);
  SET_BIT(UARTE_CONFIG(self->unit), UARTE_CONFIG_PARITYTYPE_POS);
  return;
}
 
void uarte_enableHwfc(Uarte const* const self){
  SET_BIT(UARTE_CONFIG(self->unit), UARTE_CONFIG_HWFC_POS);
  return;
}

void uarte_disableHwfc(Uarte const* const self){
  CLR_BIT(UARTE_CONFIG(self->unit), UARTE_CONFIG_HWFC_POS);
  return;
}

void uarte_enableEndRxToStartRxShort(Uarte const* const self){
  SET_BIT(UARTE_SHORTS(self->unit), UARTE_SHORTS_ENDRX_STARTRX_POS);
  return;
}

void uarte_disableEndRxToStartRxShort(Uarte const* const self){
  CLR_BIT(UARTE_SHORTS(self->unit), UARTE_SHORTS_ENDRX_STARTRX_POS);
  return;
}

void uarte_enableEndRxToStopRxShort(Uarte const* const self){
  SET_BIT(UARTE_SHORTS(self->unit), UARTE_SHORTS_ENDRX_STOPRX_POS);
  return;
}

void uarte_disableEndRxToStopRxShort(Uarte const* const self){
  CLR_BIT(UARTE_SHORTS(self->unit), UARTE_SHORTS_ENDRX_STOPRX_POS);
  return;
}


void uarte_writeChar(Uarte const* const self, char const* const ch){
  uarte_eventsReset(self);
  pUarteData[self->unit]->txdMaxCnt = 1;
  pUarteData[self->unit]->txdPtr = (uint32_t) ch;
  pUarteTask[self->unit]->startTx = 1;
  while (!(pUarteEvent[self->unit]->endTx)){}
  return;
}

void uarte_writeStr(Uarte const* const self, char const* const str){
  uarte_eventsReset(self);
  uint32_t bufferSize = strLength(str);
  char buffer[bufferSize];
  for (int i = 0; i < bufferSize; i++){
    buffer[i] = *(str + i);
  }
  pUarteData[self->unit]->txdMaxCnt = bufferSize;
  pUarteData[self->unit]->txdPtr = (uint32_t) buffer;
  pUarteTask[self->unit]->startTx = 1;
  while (!(pUarteEvent[self->unit]->endTx)){}
  return;
}

void uarte_eventsReset(Uarte const* const self){
  pUarteEvent[self->unit]->cts = 0;
  pUarteEvent[self->unit]->ncts = 0;
  pUarteEvent[self->unit]->rxdRdy = 0;
  pUarteEvent[self->unit]->endRx = 0;
  pUarteEvent[self->unit]->txdRdy = 0;
  pUarteEvent[self->unit]->endTx = 0;
  pUarteEvent[self->unit]->error = 0;
  pUarteEvent[self->unit]->rxTo = 0;
  pUarteEvent[self->unit]->rxStarted = 0;
  pUarteEvent[self->unit]->txStarted = 0;
  pUarteEvent[self->unit]->txStopped = 0;
  return;
}

void uarte_includeParity(Uarte const* const self){
  SET_FIELD(
    UARTE_CONFIG(self->unit),
    UARTE_CONFIG_PARITY_POS,
    UARTE_CONFIG_PARITY_WIDTH,
    UARTE_CONFIG_PARITY_INCLUDED
  );
  return;
}

void uarte_excludeParity(Uarte const* const self){
  SET_FIELD(
    UARTE_CONFIG(self->unit),
    UARTE_CONFIG_PARITY_POS,
    UARTE_CONFIG_PARITY_WIDTH,
    UARTE_CONFIG_PARITY_EXCLUDED
  );
  CLR_BIT(UARTE_CONFIG(self->unit), UARTE_CONFIG_PARITYTYPE_POS);
  return;
}

void uarte_configReset(Uarte const* const self){
  uarte_disableHwfc(self);
  uarte_excludeParity(self);
  uarte_setStopBits(self, STOPBITS_ONE);
  uarte_disableEndRxToStartRxShort(self);
  uarte_disableEndRxToStopRxShort(self);
  uarte_setBaudrate(self, BAUD_250000);
  return;
}

void uarte_interruptReset(Uarte const* const self){
  pUarteInterrupt[self->unit]->inten = 0;
  return;
}

void uarte_pinsReset(Uarte const* const self){
  pUartePin[self->unit]->pselRts= 0xFFFFFFFF;
  pUartePin[self->unit]->pselTxd= 0xFFFFFFFF;
  pUartePin[self->unit]->pselCts = 0xFFFFFFFF;
  pUartePin[self->unit]->pselRxd= 0xFFFFFFFF;
}

void uarte_enable(Uarte const* const self){
  SET_FIELD(UARTE_ENABLE(self->unit), UARTE_ENABLE_POS, UARTE_ENABLE_WIDTH, UARTE_ENABLE_ENABLED);
  return;
}

void uarte_disable(Uarte const* const self){
  pUarteEvent[self->unit]->txStopped = 0;
  pUarteEvent[self->unit]->rxTo = 0;
  pUarteTask[self->unit]->stopRx = 1;
  pUarteTask[self->unit]->stopTx = 1;
  while (!(pUarteEvent[self->unit]->txStopped) || !(pUarteEvent[self->unit]->rxTo)){}
  SET_FIELD(UARTE_ENABLE(self->unit), UARTE_ENABLE_POS, UARTE_ENABLE_WIDTH, UARTE_ENABLE_DISABLED);
  return;
}

void uarte_init(Uarte const* const self){
  uarte_disable(self);
  uarte_configReset(self);
  uarte_eventsReset(self);
  uarte_interruptReset(self);
  uarte_pinsReset(self);
  uarte_enable(self);
  return;
}

