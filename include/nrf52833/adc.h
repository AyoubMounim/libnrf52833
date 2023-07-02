
#ifndef ADC_H
#define ADC_H

#include <stdint.h>


typedef enum {
  ADC_STATUS_READY = 0,
  ADC_STATUS_BUSY = 0
} AdcStatus;

typedef enum {
  CH0, CH1, CH2, CH3, CH4, CH5, CH6, CH7, CH_TOTAL
} AdcChannel;

typedef enum {
  NC = 0,
  ANALOG_INPUT_0 = 1,
  ANALOG_INPUT_1 = 2,
  ANALOG_INPUT_2 = 3,
  ANALOG_INPUT_3 = 4,
  ANALOG_INPUT_4 = 5,
  ANALOG_INPUT_5 = 6,
  ANALOG_INPUT_6 = 7,
  ANALOG_INPUT_7 = 8,
  VDD = 9,
  VDDHDIV5 = 0xD
} AdcInput;


void adcInit(void);
void adcReset(void);

void adcSetResolution8bit(void);
void adcSetResolution10bit(void);
void adcSetResolution12bit(void);
void adcSetResolution14bit(void);
void adcSetOversampleBypass(void);
void adcSetOversampleOver2x(void);
void adcSetSampleRateCC(uint16_t sampleRate);
void adcSetSampleRateModeTask(void);
void adcSetSampleRateModeTimers(void);
void adcSetResultPtr(int16_t* ptr);
void adcSetMaxCnt(uint16_t maxCnt);
void adcSetChPselP(AdcChannel ch, AdcInput input);
void adcSetChPselN(AdcChannel ch, AdcInput input);

uint16_t adcGetAmount(void);
AdcStatus adcGetStatus(void);

void adcCalibrate(void);
void adcSampleStop(void);
void adcCountinuosSample(uint8_t freqKhz, int16_t* buffer, uint16_t durationMilliSec);
void adcSingleSample(int16_t* buffer);


#endif

