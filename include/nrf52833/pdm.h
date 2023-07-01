
#ifndef PDM_H
#define PDM_H

#include "gpio.h"
#include <stdint.h>

typedef enum {
  PDM_CLKCTRL_1000K = 0x08000000,
  PDM_CLKCTRL_DEFAULT = 0x08400000,
  PDM_CLKCTRL_1067K = 0x08800000,
  PDM_CLKCTRL_1231K = 0x09800000,
  PDM_CLKCTRL_1280K = 0x0A000000,
  PDM_CLKCTRL_1333K = 0x0A800000,
} PdmClkFreq;


void pdmInit(void);
void pdmReset(void);

void pdmSetClockFreq(PdmClkFreq freq);
void pdmSetModeMono(void);
void pdmSetModeStereo(void);
void pdmSetGainLeft(uint8_t gain);
void pdmSetGainRight(uint8_t gain);
void pdmSetRatio64();
void pdmSetRatio80();
void pdmSetCLKPin(Gpio const* const pin);
void pdmSetDINPin(Gpio const* const pin);
void pdmSetSamplePtr(void* ptr);
void pdmSetSampleMaxCnt(uint16_t maxCnt);

void pdmStart();
void pdmStop();

#endif

