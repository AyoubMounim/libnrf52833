
#include "nrf52833/adc.h"
#include "macros.h"
#include "nrf52833/counter.h"
#include <stdint.h>


#define ADC_BASE (0x40007000)

#define ADC_STATUS REG(uint32_t, ADC_BASE, 0x400)
#define ADC_ENABLE REG(uint32_t, ADC_BASE, 0x500)
#define ADC_ENABLE REG(uint32_t, ADC_BASE, 0x500)
#define ADC_RESOLUTION REG(uint32_t, ADC_BASE, 0x5F0)
#define ADC_OVERSAMPLE REG(uint32_t, ADC_BASE, 0x5F4)
#define ADC_SAMPLERATE REG(uint32_t, ADC_BASE, 0x5F8)
#define ADC_RESULTPTR REG(uint32_t, ADC_BASE, 0x62C)
#define ADC_RESULTMAXCNT REG(uint32_t, ADC_BASE, 0x630)
#define ADC_RESULTAMOUNT REG(uint32_t, ADC_BASE, 0x634)


enum {
  ADC_ENABLE_DISABLED = 0,
  ADC_ENABLE_ENABLED = 1
};

enum {
  ADC_RESOLUTION_8BIT = 0,
  ADC_RESOLUTION_10BIT = 1,
  ADC_RESOLUTION_12BIT = 2,
  ADC_RESOLUTION_14BIT = 3
};

enum {
  ADC_OVERSAMPLE_BYPASS = 0,
  ADC_OVERSAMPLE_OVER2X = 1,
  ADC_OVERSAMPLE_OVER4X = 2,
  ADC_OVERSAMPLE_OVER8X = 3,
  ADC_OVERSAMPLE_OVER16X = 4,
  ADC_OVERSAMPLE_OVER32X = 5,
  ADC_OVERSAMPLE_OVER64X = 6,
  ADC_OVERSAMPLE_OVER128X = 7,
  ADC_OVERSAMPLE_OVER256X = 8
};

enum {
  ADC_SAMPLERATE_CC_POS = 0,
  ADC_SAMPLERATE_CC_WIDTH = 11,
  ADC_SAMPLERATE_MODE_POS = 12,
  ADC_SAMPLERATE_MODE_WIDTH = 1,
  ADC_SAMPLERATE_MODE_TASK = 0,
  ADC_SAMPLERATE_MODE_TIMERS = 1
};


typedef struct __attribute__((packed)){
  uint32_t start;
  uint32_t sample;
  uint32_t stop;
  uint32_t calibrateOffset;
} AdcTask;

typedef struct __attribute__((packed)){
  uint32_t started;
  uint32_t end;
  uint32_t done;
  uint32_t resultDone;
  uint32_t calibrateDone;
  uint32_t stopped;
} AdcEvent;

typedef struct __attribute__((packed)){
  uint32_t chLimitH;
  uint32_t chLimitL;
} AdcChEvent;

typedef struct __attribute__((packed)){
  uint32_t pselP;
  uint32_t pselN;
  uint32_t config;
  uint32_t limit;
} AdcChConfig;

AdcTask volatile* const pAdcTask = (AdcTask*) ADC_BASE;
AdcEvent volatile* const pAdcEvent = (AdcEvent*) (ADC_BASE + 0x100);
AdcChEvent volatile* pAdcChEvent[CH_TOTAL];
AdcChConfig volatile* pAdcChConfig[CH_TOTAL];


void adcEventReset(void);
void chEventConfigSetUp(void);


void adcInit(void){
  ADC_ENABLE = ADC_ENABLE_ENABLED;
  adcEventReset();
  chEventConfigSetUp();
  return;
}

void adcReset(void){
  ADC_ENABLE = ADC_ENABLE_DISABLED;
  adcInit();
  return;
}

void adcSetResolution8bit(void){
  ADC_RESOLUTION = ADC_RESOLUTION_8BIT;
  return;
}

void adcSetResolution10bit(void){
  ADC_RESOLUTION = ADC_RESOLUTION_10BIT;
  return;
}

void adcSetResolution12bit(void){
  ADC_RESOLUTION = ADC_RESOLUTION_12BIT;
  return;
}

void adcSetResolution14bit(void){
  ADC_RESOLUTION = ADC_RESOLUTION_14BIT;
  return;
}

void adcSetOversampleBypass(void){
  ADC_OVERSAMPLE = ADC_OVERSAMPLE_BYPASS;
  return;
}

void adcSetOversampleOver2x(void){
  ADC_OVERSAMPLE = ADC_OVERSAMPLE_OVER2X;
  return;
}

void adcSetSampleRateCC(uint16_t sampleRate){
  SET_FIELD(
    ADC_SAMPLERATE,
    ADC_SAMPLERATE_CC_POS,
    ADC_SAMPLERATE_CC_WIDTH,
    sampleRate
  );
  return;
}

void adcSetSampleRateModeTask(void){
  SET_FIELD(
    ADC_SAMPLERATE,
    ADC_SAMPLERATE_MODE_POS,
    ADC_SAMPLERATE_MODE_WIDTH,
    ADC_SAMPLERATE_MODE_TASK
  );
  return;
}

void adcSetSampleRateModeTimers(void){
  SET_FIELD(
    ADC_SAMPLERATE,
    ADC_SAMPLERATE_MODE_POS,
    ADC_SAMPLERATE_MODE_WIDTH,
    ADC_SAMPLERATE_MODE_TIMERS
  );
  return;
}

void adcSetResultPtr(int16_t* ptr){
  ADC_RESULTPTR = (uint32_t) ptr;
  return;
}

void adcSetMaxCnt(uint16_t maxCnt){
  ADC_RESULTMAXCNT = maxCnt;
  return;
}

void adcSetChPselP(AdcChannel ch, AdcInput input){
  pAdcChConfig[ch]->pselP = (uint32_t) input;
  return;
}

void adcSetChPselN(AdcChannel ch, AdcInput input){
  pAdcChConfig[ch]->pselN = (uint32_t) input;
  return;
}


uint16_t adcGetAmount(void){
  return ADC_RESULTAMOUNT;
}

AdcStatus adcGetStatus(void){
  AdcStatus status = ADC_STATUS;
  return status;
}


void adcCalibrate(void){
  pAdcTask->calibrateOffset = 1;
  while (!(pAdcEvent->calibrateDone)){}
  return;
}

void adcSampleStop(void){
  pAdcTask->stop = 1;
  while (!(pAdcEvent->stopped)){}
  return;
}

void adcCountinuosSample(uint8_t freqKhz, int16_t* buffer, uint16_t durationMilliSec){
  adcEventReset();
  adcSetResultPtr(buffer);
  adcSetSampleRateModeTimers();
  uint16_t cc = 16000/freqKhz; 
  adcSetSampleRateCC(cc);
  pAdcTask->start = 1;
  while (!(pAdcEvent->started)){}
  sleep(durationMilliSec);
  adcSampleStop();
  return;
}

void adcSingleSample(int16_t* buffer){
  adcEventReset();
  adcSetResultPtr(buffer);
  adcSetMaxCnt(1);  
  adcSetSampleRateModeTask();
  pAdcTask->start = 1;
  while (!(pAdcEvent->started)){}
  pAdcTask->sample = 1;
  while (!(pAdcEvent->end)){}
  adcSampleStop();
  return;
}


void adcEventReset(void){
  pAdcEvent-> started = 0;
  pAdcEvent-> stopped = 0;
  pAdcEvent-> calibrateDone = 0;
  pAdcEvent-> resultDone = 0;
  pAdcEvent-> end = 0;
  for (uint8_t ch = 0; ch < CH_TOTAL; ch++){
    pAdcChEvent[ch]->chLimitH = 0;
    pAdcChEvent[ch]->chLimitL = 0;
  }
  return;
}

void chEventConfigSetUp(void){
  for (uint8_t ch = 0; ch < CH_TOTAL; ch++){
    pAdcChEvent[ch] = (AdcChEvent*) (ADC_BASE + 0x118 + ch*0x8);
    pAdcChConfig[ch] = (AdcChConfig*) (ADC_BASE + 0x510 + ch*0x10);
  }
  return;
}

