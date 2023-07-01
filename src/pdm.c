
#include "nrf52833/pdm.h"
#include "macros.h"
#include "nrf52833/gpio.h"
#include <stdint.h>


#define PDM_BASE (0x4001D000)

#define PDM_ENABLE REG(uint32_t, PDM_BASE, 0x500)
#define PDM_CLKCTRL REG(uint32_t, PDM_BASE, 0x504)
#define PDM_MODE REG(uint32_t, PDM_BASE, 0x508)
#define PDM_GAINL REG(uint32_t, PDM_BASE, 0x518)
#define PDM_GAINR REG(uint32_t, PDM_BASE, 0x51C)
#define PDM_RATIO REG(uint32_t, PDM_BASE, 0x520)
#define PDM_PSELCLK REG(uint32_t, PDM_BASE, 0x540)
#define PDM_PSELDIN REG(uint32_t, PDM_BASE, 0x544)
#define PDM_SAMPLEPTR REG(uint32_t, PDM_BASE, 0x560)
#define PDM_SAMPLEMAXCNT REG(uint32_t, PDM_BASE, 0x564)

enum {
  PDM_ENABLE_DISABLED = 0,
  PDM_ENABLE_ENABLED = 1,
};

enum {
  PDM_MODE_OPERATION_POS = 0,
  PDM_MODE_OPERATION_WIDTH = 1,
  PDM_MODE_OPERATION_STEREO = 0,
  PDM_MODE_OPERATION_MONO = 1,
  PDM_MODE_EDGE_POS = 1,
  PDM_MODE_EDGE_WIDTH = 1,
  PDM_MODE_EDGE_LEFTFALLING = 0,
  PDM_MODE_EDGE_LEFTRISING = 1,
};

enum {
  PDM_RATIO_RATIO64 = 0,
  PDM_RATIO_RATIO80 = 1,
};

enum {
  PDM_PSEL_PIN_POS = 0,
  PDM_PSEL_PIN_WIDTH = 5,
  PDM_PSEL_PORT_POS = 5,
  PDM_PSEL_PORT_WIDTH = 1,
  PDM_PSEL_CONNECT_POS = 31,
  PDM_PSEL_CONNECT_WIDTH = 1,
  PDM_PSEL_CONNECT_DISCONNECTED = 1,
  PDM_PSEL_CONNECT_CONNECTED = 0,
};


typedef struct __attribute__((packed)){
  uint32_t start;
  uint32_t stop;
} PdmTask;

typedef struct __attribute__((packed)){
  uint32_t started;
  uint32_t stopped;
  uint32_t end;
} PdmEvent;

typedef struct __attribute__((packed)){
  uint32_t inten;
  uint32_t intenSet;
  uint32_t intenClr;
} PdmInterrupt;

PdmTask volatile* const pPdmTask = (PdmTask volatile*) (PDM_BASE);
PdmEvent volatile* const pPdmEvent = (PdmEvent volatile*) (PDM_BASE + 0x100);
PdmInterrupt volatile* const pPdmInterrupt = (PdmInterrupt volatile*) (PDM_BASE + 0x300);



void pdm_eventsReset(void);


void pdmReset(void){
  pdmInit();
  return;
}

void pdmInit(void){
  PDM_ENABLE = PDM_ENABLE_DISABLED;
  pdm_eventsReset();
  pdmSetClockFreq(PDM_CLKCTRL_DEFAULT);
  pdmSetModeStereo();
  pdmSetGainLeft(0x28);
  pdmSetGainRight(0x28);
  pdmSetRatio64();
  SET_BIT(PDM_PSELCLK, PDM_PSEL_CONNECT_POS);
  SET_BIT(PDM_PSELDIN, PDM_PSEL_CONNECT_POS);
  PDM_SAMPLEPTR = 0x0;
  PDM_SAMPLEMAXCNT = 0x0;
  PDM_ENABLE = PDM_ENABLE_ENABLED;
  return;
}


void pdm_eventsReset(void){
  pPdmEvent->started = 0;
  pPdmEvent->stopped = 0;
  pPdmEvent->end = 0;
  return;
}


void pdmSetClockFreq(PdmClkFreq freq){
  PDM_CLKCTRL = (uint32_t) freq;
  return;
}

void pdmSetModeMono(void){
  SET_FIELD(
    PDM_MODE,
    PDM_MODE_OPERATION_POS,
    PDM_MODE_OPERATION_WIDTH,
    PDM_MODE_OPERATION_MONO
  );
  return;
}

void pdmSetModeStereo(void){
  SET_FIELD(
    PDM_MODE,
    PDM_MODE_OPERATION_POS,
    PDM_MODE_OPERATION_WIDTH,
    PDM_MODE_OPERATION_STEREO
  );
  return;
}

void pdmSetGainLeft(uint8_t gain){
  PDM_GAINL = gain;
  return;
}

void pdmSetGainRight(uint8_t gain){
  PDM_GAINR = gain;
  return;
}

void pdmSetRatio64(){
  PDM_RATIO = PDM_RATIO_RATIO64;
  return;
}

void pdmSetRatio80(){
  PDM_RATIO = PDM_RATIO_RATIO80;
  return;
}

void pdmSetCLKPin(Gpio const* const pin){
  SET_FIELD(
    PDM_PSELCLK,
    PDM_PSEL_PIN_POS,
    PDM_PSEL_PIN_WIDTH,
    pin->pin
  );
  SET_FIELD(
    PDM_PSELCLK,
    PDM_PSEL_PORT_POS,
    PDM_PSEL_PORT_WIDTH,
    pin->port
  );
  SET_FIELD(
    PDM_PSELCLK,
    PDM_PSEL_CONNECT_POS,
    PDM_PSEL_CONNECT_WIDTH,
    PDM_PSEL_CONNECT_CONNECTED
  );
  gpio_configOutput(pin);
  gpio_setLow(pin);
  gpio_configConnect(pin);
  return;
}

void pdmSetDINPin(Gpio const* const pin){
  SET_FIELD(
    PDM_PSELDIN,
    PDM_PSEL_PIN_POS,
    PDM_PSEL_PIN_WIDTH,
    pin->pin
  );
  SET_FIELD(
    PDM_PSELDIN,
    PDM_PSEL_PORT_POS,
    PDM_PSEL_PORT_WIDTH,
    pin->port
  );
  SET_FIELD(
    PDM_PSELDIN,
    PDM_PSEL_CONNECT_POS,
    PDM_PSEL_CONNECT_WIDTH,
    PDM_PSEL_CONNECT_CONNECTED
  );
  gpio_configInput(pin);
  gpio_configConnect(pin);
  return;
}

void pdmSetSamplePtr(void* ptr){
  PDM_SAMPLEPTR = (uint32_t) ptr;
  return;
}

void pdmSetSampleMaxCnt(uint16_t maxCnt){
  PDM_SAMPLEMAXCNT = maxCnt;
  return;
}


void pdmStart(){
  pPdmTask->start = 1;
  while (!(pPdmEvent->started)){}
  return;
}

void pdmStop(){
  pPdmTask->stop = 1;
  while (!(pPdmEvent->stopped)){}
  return;
}

