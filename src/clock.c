
#include "clock.h"
#include "macros.h"
#include <stdint.h>


#define CLOCK_BASE 0x40000000

#define CLOCK_LFCLKSRC       REG(uint32_t, CLOCK_BASE, 0x518)
#define CLOCK_HFXODEBOUNCE   REG(uint32_t, CLOCK_BASE, 0x528)
#define CLOCK_LFXODEBOUNCE   REG(uint32_t, CLOCK_BASE, 0x52C)
#define CLOCK_CTIV           REG(uint32_t, CLOCK_BASE, 0x538)
#define CLOCK_TRACECONFIG    REG(uint32_t, CLOCK_BASE, 0x55C)


enum {
  CLOCK_LFCLKSRC_SRC_RC = 0,
  CLOCK_LFCLKSRC_SRC_XTAL = 1,
  CLOCK_LFCLKSRC_SRC_SYNTH = 2,
  CLOCK_LFCLKSRC_SRC_POS = 0,
  CLOCKLFCLKSRC_SRC_WIDTH = 2,
  CLOCK_LFCLKSRC_BYPASS_POS = 16,
  CLOCK_LFCLKSRC_EXTERNAL_POS = 17,
};

enum {
  CLOCK_TRACECONFIG_PORTSPEED_32Mhz = 0,
  CLOCK_TRACECONFIG_PORTSPEED_16Mhz = 1,
  CLOCK_TRACECONFIG_PORTSPEED_8Mhz = 2,
  CLOCK_TRACECONFIG_PORTSPEED_4Mhz = 3,
  CLOCK_TRACECONFIG_PORTSPEED_WIDTH = 2,
  CLOCK_TRACECONFIG_PORTSPEED_POS = 0,
  CLOCK_TRACECONFIG_TRACEMUX_GPIO = 0,
  CLOCK_TRACECONFIG_TRACEMUX_SERIAL = 1,
  CLOCK_TRACECONFIG_TRACEMUX_PARALLEL = 2,
  CLOCK_TRACECONFIG_TRACEMUX_WIDTH = 2,
  CLOCK_TRACECONFIG_TRACEMUX_POS = 16,
};


typedef struct {
  uint32_t hfClkStart;
  uint32_t hfClkStop;
  uint32_t lfClkStart;
  uint32_t lfClkStop;
  uint32_t cal;
  uint32_t ctStart;
  uint32_t ctStop;
} ClockTask;


typedef struct {
  uint32_t hfClkStarted;
  uint32_t lfClkStarted;
  uint32_t _reserved0;
  uint32_t done;
  uint32_t ctto;
  uint64_t _reserved1;
  uint64_t _reserved2;
  uint32_t _reserved3;
  uint32_t ctStarted;
  uint32_t ctStopped;
} ClockEvent;


typedef struct {
  uint32_t intenSet;
  uint32_t intenClr;
} ClockInterrupt;


typedef struct {
  uint32_t hfClkRun;
  uint32_t hfClkStat;
  uint32_t _reserved0;
  uint32_t lfClkRun;
  uint32_t lfClkStat;
  uint32_t lfClkSrcCopy;
} ClockStatus;


ClockTask* pClockTask = (ClockTask*) CLOCK_BASE;
ClockEvent* pClockEvent = (ClockEvent*) (CLOCK_BASE + 0x100);
ClockInterrupt* pClockInterrupt = (ClockInterrupt*) (CLOCK_BASE + 0x304);
ClockStatus* pClockStatus = (ClockStatus*) (CLOCK_BASE + 0x40C);


void hfClockInit(void){
  hfClockStop();
  pClockEvent->hfClkStarted = 0;
  return;
}

void hfClockStart(void){
  pClockTask->hfClkStart = 1;
  while (!(pClockEvent->hfClkStarted)){};
  return;
}

void hfClockStop(void){
    pClockTask->hfClkStop = 1;
    return;
}

void hfClockSetDebounce(uint8_t debounce){
  CLOCK_HFXODEBOUNCE = (uint32_t) debounce;
  return;
}

void lfClockInit(void){
  lfClockStop();
  pClockEvent->lfClkStarted = 0;
  return;
}

void lfClockConfigRC(void){
  SET_FIELD(CLOCK_LFCLKSRC, CLOCK_LFCLKSRC_SRC_POS, CLOCKLFCLKSRC_SRC_WIDTH, CLOCK_LFCLKSRC_SRC_RC);
  CLR_BIT(CLOCK_LFCLKSRC, CLOCK_LFCLKSRC_BYPASS_POS);
  CLR_BIT(CLOCK_LFCLKSRC, CLOCK_LFCLKSRC_EXTERNAL_POS);
  return;
}

void lfClockConfigXtal(void){
  SET_FIELD(CLOCK_LFCLKSRC, CLOCK_LFCLKSRC_SRC_POS, CLOCKLFCLKSRC_SRC_WIDTH, CLOCK_LFCLKSRC_SRC_XTAL);
  CLR_BIT(CLOCK_LFCLKSRC, CLOCK_LFCLKSRC_BYPASS_POS);
  CLR_BIT(CLOCK_LFCLKSRC, CLOCK_LFCLKSRC_EXTERNAL_POS);
  return;
}

void lfClockConfigSynth(void){
  SET_FIELD(CLOCK_LFCLKSRC, CLOCK_LFCLKSRC_SRC_POS, CLOCKLFCLKSRC_SRC_WIDTH, CLOCK_LFCLKSRC_SRC_SYNTH);
  CLR_BIT(CLOCK_LFCLKSRC, CLOCK_LFCLKSRC_BYPASS_POS);
  CLR_BIT(CLOCK_LFCLKSRC, CLOCK_LFCLKSRC_EXTERNAL_POS);
  return;
}

void lfClockStart(void){
  pClockTask->lfClkStart = 1;
  while (!(pClockEvent->lfClkStarted)){};
  return;
}

void lfClockStop(void){
  pClockTask->lfClkStop = 1;
  return;
}

void lfClockSetDebounceNormal(void){
    CLOCK_LFXODEBOUNCE = 0;
    return;
}

void lfClockSetDebounceExtended(void){
    CLOCK_LFXODEBOUNCE = 1;
    return;
}


void setCTIV(uint8_t calibrationInterval){
    CLOCK_CTIV = (uint32_t) calibrationInterval;
    return;
}


void sleep(uint32_t usec){
    uint32_t t = usec << 20;
    while (t > 0){
        nop();
        t--;
    }
    return;
}

