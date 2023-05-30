
#include "nrf52833/i2c.h"
#include "macros.h"
#include "nrf52833/gpio.h"
#include <stdint.h>


#define TWIM_BASE(unit) (0x40003000 + (unit)*0x1000)

#define TWIM_SHORTS(unit) REG(uint32_t, TWIM_BASE(unit), 0x200)
#define TWIM_ERRORSRC(unit) REG(uint32_t, TWIM_BASE(unit), 0x4C4)
#define TWIM_ENABLE(unit) REG(uint32_t, TWIM_BASE(unit), 0x500)
#define TWIM_FREQUENCY(unit) REG(uint32_t, TWIM_BASE(unit), 0x524)
#define TWIM_ADDRESS(unit) REG(uint32_t, TWIM_BASE(unit), 0x524)

#define ENABLED 0x6
#define DISABLED 0x0

enum {
  TWIM_SHORTS_LASTTX_STARTRX_POS = 7,
  TWIM_SHORTS_LASTTX_SUSPEND_POS = 8,
  TWIM_SHORTS_LASTTX_STOP_POS = 9,
  TWIM_SHORTS_LASTRX_STARTTX_POS = 10,
  TWIM_SHORTS_LASTRX_SUSPEND_POS = 11,
  TWIM_SHORTS_LASTRX_STOP_POS = 12,
};

enum {
  TWIM_ERRORSRC_OVERRUN_POS = 0,
  TWIM_ERRORSRC_ANACK_POS = 1,
  TWIM_ERRORSRC_DNACK_POS = 2,
};

enum {
  TWIM_ENABLE_ENABLED = ENABLED,
  TWIM_ENABLE_DISABLED = DISABLED,
  TWIM_ENABLE_WIDTH = 4,
  TWIM_ENABLE_POS = 0
};


typedef struct __attribute__((packed)){
  uint32_t startRx;
  uint32_t _reserved0;
  uint32_t startTx;
  uint64_t _reserved1;
  uint32_t stop;
  uint32_t _reserved2;
  uint32_t suspend;
  uint32_t resume;
} TwimTask;

typedef struct __attribute__((packed)){
  uint32_t stopped;
  uint64_t _reserved0;
  uint64_t _reserved1;
  uint32_t error;
  uint64_t _reserved2;
  uint64_t _reserved3;
  uint32_t _reserved4;
  uint32_t suspended;
  uint32_t rxStarted;
  uint32_t txStarted;
  uint64_t _reserved5;
  uint32_t lastRx;
  uint32_t lastTx;
} TwimEvent;

typedef struct __attribute__((packed)){
  uint32_t inten;
  uint32_t intenSet;
  uint32_t intenClr;
} TwimInterrupt;

typedef struct __attribute__((packed)){
  uint32_t pselScl;
  uint32_t pselSda;
} TwimPin;

typedef struct __attribute__((packed)){
  uint32_t rxdPtr;
  uint32_t rxdMaxCnt;
  uint32_t rxdAmount;
  uint32_t rxdList;
  uint32_t txdPtr;
  uint32_t txdMaxCnt;
  uint32_t txdAmount;
  uint32_t txdList;
} TwimData;


TwimTask volatile* pTwimTask[] = {
  (TwimTask volatile*) (TWIM_BASE(UNIT_0) + 0x0),
  (TwimTask volatile*) (TWIM_BASE(UNIT_1) + 0x0)
};
TwimEvent volatile* pTwimEvent[] = {
  (TwimEvent volatile*) (TWIM_BASE(UNIT_0) + 0x104),
  (TwimEvent volatile*) (TWIM_BASE(UNIT_1) + 0x104)
};
TwimInterrupt volatile* pTwimInterrupt[] = {
  (TwimInterrupt volatile*) (TWIM_BASE(UNIT_0) + 0x300),
  (TwimInterrupt volatile*) (TWIM_BASE(UNIT_1) + 0x300)
};
TwimPin volatile* pTwimPin[] = {
  (TwimPin volatile*) (TWIM_BASE(UNIT_0) + 0x508),
  (TwimPin volatile*) (TWIM_BASE(UNIT_1) + 0x508)
};
TwimData volatile* pTwimData[] = {
  (TwimData volatile*) (TWIM_BASE(UNIT_0) + 0x534),
  (TwimData volatile*) (TWIM_BASE(UNIT_1) + 0x534)
};


void twim_init(Twim const* const self);
void twim_eventsReset(Twim const* const self);

Twim twim_create(Unit unit, Gpio const* const sclPin, Gpio const* const sdaPin){
  Twim twim = {.unit=unit};
  twim_init(&twim);
  twim_setSclPin(&twim, sclPin);
  twim_setSdaPin(&twim, sdaPin);
  return twim;
}

void twim_reset(Twim const* const self){
  twim_stop(self);
  twim_init(self);
  return;
}


void twim_enable(Twim const* const self){
  SET_FIELD(
    TWIM_ENABLE(self->unit),
    TWIM_ENABLE_POS,
    TWIM_ENABLE_WIDTH,
    TWIM_ENABLE_ENABLED
  );
  return;
}

void twim_disable(Twim const* const self){
  SET_FIELD(
    TWIM_ENABLE(self->unit),
    TWIM_ENABLE_POS,
    TWIM_ENABLE_WIDTH,
    TWIM_ENABLE_DISABLED
  );
  return;
}


uint8_t twim_isStarted(Twim const* const self){
  uint8_t isStarted;
  if (pTwimEvent[self->unit]->txStarted || pTwimEvent[self->unit]->rxStarted){
    isStarted = 1;
  }
  else {
    isStarted = 0;
  }
  return  isStarted;
}

uint8_t twim_isEnabled(Twim const* const self){
  uint8_t isEnabled;
  uint8_t regEnabledValue = GET_FIELD(
    TWIM_ENABLE(self->unit),
    TWIM_ENABLE_POS,
    TWIM_ENABLE_WIDTH
  );
  if (regEnabledValue == ENABLED){
    isEnabled = 1;
  }
  else {
    isEnabled = 0;
  }
  return  isEnabled;
}

uint8_t twim_isSuspended(Twim const* const self){
  uint8_t isSuspended;
  if (pTwimEvent[self->unit]->suspended){
    isSuspended = 1;
  }
  else {
    isSuspended = 0; 
  }
  return isSuspended;
}


void twim_resume(Twim const* const self){
  if (!twim_isSuspended(self)){
    return;
  }
  pTwimTask[self->unit]->resume = 1;
  pTwimEvent[self->unit]->suspended = 0;
  return;
}

void twim_stop(Twim const* const self){
  if (!twim_isEnabled(self)){
    return;
  }
  if (!twim_isStarted(self)){
    return;
  }
  if (twim_isSuspended(self)){
    twim_resume(self);
    twim_stop(self);
    return;
  }
  pTwimTask[self->unit]->stop = 1;
  while (!(pTwimEvent[self->unit]->stopped)){}
  twim_eventsReset(self);
  return;
}


void twim_setSclPin(Twim const* const self, Gpio const* const pin){
  twim_stop(self);
  twim_disable(self);
  gpio_configDisconnect(pin);
  gpio_configInput(pin);
  gpio_configConnect(pin);
  pTwimPin[self->unit]->pselScl = ((pin->port) << 5) + pin->pin;
  twim_enable(self);
  return;
}

void twim_setSdaPin(Twim const* const self, Gpio const* const pin){
  twim_stop(self);
  twim_disable(self);
  gpio_configDisconnect(pin);
  gpio_configInput(pin);
  gpio_configConnect(pin);
  pTwimPin[self->unit]->pselSda = ((pin->port) << 5) + pin->pin;
  twim_enable(self);
  return;
}

void twim_setFrequency(Twim const* const self, TwimFreq freq){
  TWIM_FREQUENCY(self->unit) = freq;
  return;
}

void twim_setAddress(Twim const* const self, uint32_t addr){
  TWIM_ADDRESS(self->unit) = addr;
  return;
}


void twim_write(Twim const* const self, void* dataPtr, uint32_t dataLen, uint32_t slaveAddr){
  twim_stop(self);
  twim_eventsReset(self);
  twim_shortLastTxToStopEnable(self);
  twim_setAddress(self, slaveAddr);
  pTwimData[self->unit]->txdPtr = (uint32_t) dataPtr;
  pTwimData[self->unit]->txdMaxCnt = dataLen;
  pTwimTask[self->unit]->startTx = 1;
  while (!(pTwimEvent[self->unit]->stopped)){}
  twim_eventsReset(self);
  return;
}

void twim_read(Twim const* const self, void* bufferPtr, uint32_t bufferLen, uint32_t slaveAddr){
  twim_stop(self);
  twim_eventsReset(self);
  twim_shortLastRxToStopEnable(self);
  twim_setAddress(self, slaveAddr);
  pTwimData[self->unit]->rxdPtr = (uint32_t) bufferPtr;
  pTwimData[self->unit]->rxdMaxCnt = bufferLen;
  pTwimTask[self->unit]->startRx = 1;
  while (!(pTwimEvent[self->unit]->stopped)){}
  twim_eventsReset(self);
  return;
}

void twim_shortLastTxToStartRxEnable(Twim const* const self){
  SET_BIT(
    TWIM_SHORTS(self->unit),
    TWIM_SHORTS_LASTTX_STARTRX_POS
  );
  return;
}

void twim_shortLastTxToStartRxDisable(Twim const* const self){
  CLR_BIT(
    TWIM_SHORTS(self->unit),
    TWIM_SHORTS_LASTTX_STARTRX_POS
  );
  return;
}

void twim_shortLastTxToSuspendEnable(Twim const* const self){
  SET_BIT(
    TWIM_SHORTS(self->unit),
    TWIM_SHORTS_LASTTX_SUSPEND_POS
  );
  return;
}

void twim_shortLastTxToSuspendDisable(Twim const* const self){
  CLR_BIT(
    TWIM_SHORTS(self->unit),
    TWIM_SHORTS_LASTTX_SUSPEND_POS
  );
  return;
}

void twim_shortLastTxToStopEnable(Twim const* const self){
  SET_BIT(
    TWIM_SHORTS(self->unit),
    TWIM_SHORTS_LASTTX_STOP_POS
  );
  return;
}

void twim_shortLastTxToStopDisable(Twim const* const self){
  CLR_BIT(
    TWIM_SHORTS(self->unit),
    TWIM_SHORTS_LASTTX_STOP_POS
  );
  return;
}

void twim_shortLastRxToStartTxEnable(Twim const* const self){
  SET_BIT(
    TWIM_SHORTS(self->unit),
    TWIM_SHORTS_LASTRX_STARTTX_POS
  );
  return;
}

void twim_shortLastRxToStartTxDisable(Twim const* const self){
  CLR_BIT(
    TWIM_SHORTS(self->unit),
    TWIM_SHORTS_LASTRX_STARTTX_POS
  );
  return;
}

void twim_shortLastRxToSuspendEnable(Twim const* const self){
  SET_BIT(
    TWIM_SHORTS(self->unit),
    TWIM_SHORTS_LASTRX_SUSPEND_POS
  );
  return;
}

void twim_shortLastRxToSuspendDisable(Twim const* const self){
  CLR_BIT(
    TWIM_SHORTS(self->unit),
    TWIM_SHORTS_LASTRX_SUSPEND_POS
  );
  return;
}

void twim_shortLastRxToStopEnable(Twim const* const self){
  SET_BIT(
    TWIM_SHORTS(self->unit),
    TWIM_SHORTS_LASTRX_STOP_POS
  );
  return;
}

void twim_shortLastRxToStopDisable(Twim const* const self){
  CLR_BIT(
    TWIM_SHORTS(self->unit),
    TWIM_SHORTS_LASTRX_STOP_POS
  );
  return;
}


void twim_eventsReset(Twim const* const self){
  pTwimEvent[self->unit]->stopped = 0;
  pTwimEvent[self->unit]->error = 0;
  pTwimEvent[self->unit]->suspended = 0;
  pTwimEvent[self->unit]->rxStarted = 0;
  pTwimEvent[self->unit]->txStarted = 0;
  pTwimEvent[self->unit]->lastRx = 0;
  pTwimEvent[self->unit]->lastTx = 0;
  return;
}

void twim_init(Twim const* const self){
  twim_stop(self);
  twim_disable(self);
  twim_eventsReset(self);
  twim_shortLastTxToStartRxDisable(self);
  twim_shortLastTxToSuspendDisable(self);
  twim_shortLastTxToStopDisable(self);
  twim_shortLastRxToStartTxDisable(self);
  twim_shortLastRxToSuspendDisable(self);
  twim_shortLastRxToStopDisable(self);
  CLR_BIT(TWIM_ERRORSRC(self->unit), TWIM_ERRORSRC_OVERRUN_POS);
  CLR_BIT(TWIM_ERRORSRC(self->unit), TWIM_ERRORSRC_ANACK_POS);
  CLR_BIT(TWIM_ERRORSRC(self->unit), TWIM_ERRORSRC_DNACK_POS);
  pTwimPin[self->unit]->pselScl = 0xFFFFFFFF;
  pTwimPin[self->unit]->pselSda = 0xFFFFFFFF;
  twim_setFrequency(self, K250);
  pTwimData[self->unit]->rxdPtr = 0x0;
  pTwimData[self->unit]->rxdMaxCnt = 0x0;
  pTwimData[self->unit]->rxdAmount = 0x0;
  pTwimData[self->unit]->rxdList = 0x0;
  pTwimData[self->unit]->txdPtr = 0x0;
  pTwimData[self->unit]->txdMaxCnt = 0x0;
  pTwimData[self->unit]->txdAmount = 0x0;
  pTwimData[self->unit]->txdList = 0x0;
  twim_setAddress(self, 0x0); 
  twim_enable(self);
  return;
}

