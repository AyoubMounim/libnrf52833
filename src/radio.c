
#include "nrf52833/radio.h"
#include "macros.h"
#include "nrf52833/pwm.h"
#include "nrf52833/counter.h"
#include <stdint.h>


#define RADIO_BASE 0x40001000

#define RADIO_SHORTS REG(uint32_t, RADIO_BASE, 0x200)
#define RADIO_RXMATCH REG(uint32_t, RADIO_BASE, 0x408)
#define RADIO_RXCRC REG(uint32_t, RADIO_BASE, 0x40C)
#define RADIO_DAI REG(uint32_t, RADIO_BASE, 0x410)
#define RADIO_PDUSTAT REG(uint32_t, RADIO_BASE, 0x414)
#define RADIO_CTESTATUS REG(uint32_t, RADIO_BASE, 0x44C)
#define RADIO_DFESTATUS REG(uint32_t, RADIO_BASE, 0x458)
#define RADIO_PACKETPTR REG(uint32_t, RADIO_BASE, 0x504)
#define RADIO_FREQUENCY REG(uint32_t, RADIO_BASE, 0x508)
#define RADIO_TXPOWER REG(uint32_t, RADIO_BASE, 0x50C)
#define RADIO_MODE REG(uint32_t, RADIO_BASE, 0x510)
#define RADIO_PCNF0 REG(uint32_t, RADIO_BASE, 0x514)
#define RADIO_PCNF1 REG(uint32_t, RADIO_BASE, 0x518)
#define RADIO_BASE0 REG(uint32_t, RADIO_BASE, 0x51C)
#define RADIO_BASE1 REG(uint32_t, RADIO_BASE, 0x520)
#define RADIO_PREFIX0 REG(uint32_t, RADIO_BASE, 0x524)
#define RADIO_PREFIX1 REG(uint32_t, RADIO_BASE, 0x528)
#define RADIO_TXADDRESS REG(uint32_t, RADIO_BASE, 0x52C)
#define RADIO_RXADDRESSES REG(uint32_t, RADIO_BASE, 0x530)
#define RADIO_CRCCNF REG(uint32_t, RADIO_BASE, 0x534)
#define RADIO_CRCPOLY REG(uint32_t, RADIO_BASE, 0x538)
#define RADIO_CRCINIT REG(uint32_t, RADIO_BASE, 0x53C)
#define RADIO_TIFS REG(uint32_t, RADIO_BASE, 0x544)
#define RADIO_RSSISAMPLE REG(uint32_t, RADIO_BASE, 0x548)
#define RADIO_STATE REG(uint32_t, RADIO_BASE, 0x550)
#define RADIO_DATAWHITEIV REG(uint32_t, RADIO_BASE, 0x554)
#define RADIO_BCC REG(uint32_t, RADIO_BASE, 0x560)
#define RADIO_DACNF REG(uint32_t, RADIO_BASE, 0x640)
#define RADIO_MHRMATCHCONGIG REG(uint32_t, RADIO_BASE, 0x644)
#define RADIO_MHRMATCHMAS REG(uint32_t, RADIO_BASE, 0x648)
#define RADIO_MODECNF0 REG(uint32_t, RADIO_BASE, 0x650)
#define RADIO_SFD REG(uint32_t, RADIO_BASE, 0x660)
#define RADIO_EDCNT REG(uint32_t, RADIO_BASE, 0x664)
#define RADIO_EDSAMPLE REG(uint32_t, RADIO_BASE, 0x668)
#define RADIO_CCACTRL REG(uint32_t, RADIO_BASE, 0x66C)
#define RADIO_DFEMODE REG(uint32_t, RADIO_BASE, 0x900)
#define RADIO_CTEINLINECONF REG(uint32_t, RADIO_BASE, 0x904) 
#define RADIO_DFECTRL1 REG(uint32_t, RADIO_BASE, 0x910) 
#define RADIO_DFECTRL2 REG(uint32_t, RADIO_BASE, 0x914) 
#define RADIO_SWITCHPATTERN REG(uint32_t, RADIO_BASE, 0x924) 
#define RADIO_CLEARPATTERN REG(uint32_t, RADIO_BASE, 0x924) 
#define RADIO_DFEPACKETPTR REG(uint32_t, RADIO_BASE, 0x950)
#define RADIO_DFEPACKETMAXCNT REG(uint32_t, RADIO_BASE, 0x954)
#define RADIO_DFEPACKETAMOUNT REG(uint32_t, RADIO_BASE, 0x958)
#define RADIO_POWER REG(uint32_t, RADIO_BASE, 0xFFC)

#define RADIO_PSELDFEGPIO(n) REG(uint32_t, RADIO_BASE, 0x930 + (n))


enum {
  RADIO_SHORTS_READY_START_POS = 0,
  RADIO_SHORTS_END_DISABLE_POS = 1,
  RADIO_SHORTS_DISABLE_TXEN_POS = 2,
  RADIO_SHORTS_DISABLE_RXEN_POS = 3,
  RADIO_SHORTS_ADDRESS_RSSISTART_POS = 4,
  RADIO_SHORTS_END_START_POS = 5,
  RADIO_SHORTS_ADDRESS_BCSTART_POS = 6,
  RADIO_SHORTS_DISABLED_RSSISTOP_POS = 8,
  RADIO_SHORTS_RXREADY_CCASTART_POS = 11,
  RADIO_SHORTS_CCAIDLE_TXEN_POS = 12,
  RADIO_SHORTS_CCABUSY_DISABLE_POS = 13,
  RADIO_SHORTS_FRAMESTART_BCSTART_POS = 14,
  RADIO_SHORTS_READY_EDSTART_POS = 15,
  RADIO_SHORTS_EDEND_DISABLE_POS = 16,
  RADIO_SHORTS_CCAIDLE_STOP_POS = 17,
  RADIO_SHORTS_TXREADY_START_POS = 18,
  RADIO_SHORTS_RXREADY_START_POS = 19,
  RADIO_SHORTS_PHYEND_DISABLE_POS = 20,
  RADIO_SHORTS_PHYEND_START_POS = 21,
};

enum {
  RADIO_FREQUENCY_FREQ_WIDTH = 7,
  RADIO_FREQUENCY_FREQ_POS = 0,
  RADIO_FREQUENCY_MAP_POS = 8
};

enum {
  RADIO_PCNF0_LFLEN_WIDTH = 4,
  RADIO_PCNF0_LFLEN_POS = 0,
  RADIO_PCNF0_S0LEN_POS = 8,
  RADIO_PCNF0_S1LEN_WIDTH = 4,
  RADIO_PCNF0_S1LEN_POS = 16,
  RADIO_PCNF0_S1INCL_POS = 20,
  RADIO_PCNF0_CILEN_WIDTH = 2,
  RADIO_PCNF0_CILEN_POS = 22,
  RADIO_PCNF0_PLEN_WIDTH = 2,
  RADIO_PCNF0_PLEN_POS = 24,
  RADIO_PCNF0_CRCINC_POS = 26,
  RADIO_PCNF0_TERMLEN_WIDTH = 2,
  RADIO_PCNF0_TERMLEN_POS = 29,
};

enum {
  RADIO_PCNF1_MAXLEN_WIDTH = 8,
  RADIO_PCNF1_MAXLEN_POS = 0,
  RADIO_PCNF1_STATLEN_WIDTH = 8,
  RADIO_PCNF1_STATLEN_POS = 8,
  RADIO_PCNF1_BALEN_WIDTH = 3,
  RADIO_PCNF1_BALEN_POS = 16,
  RADIO_PCNF1_ENDIAN_POS = 24,
  RADIO_PCNF1_WHITEEN_POS = 25,
};

enum {
  RADIO_CRCCNF_LEN_WIDTH = 2,
  RADIO_CRCCNF_LEN_POS = 0,
  RADIO_CRCCNF_SKIPADDR_WIDTH = 2,
  RADIO_CRCCNF_SKIPADDR_POS = 8,
};

enum {
  RADIO_MODECNF0_RU_POS = 0,
  RADIO_MODECNF0_DTX_WIDTH = 2,
  RADIO_MODECNF0_DTX_POS = 8,
  RADIO_MODECNF0_DTX_B0 = 1,
  RADIO_MODECNF0_DTX_B1 = 0,
  RADIO_MODECNF0_DTX_CENTER = 2,
};

enum {
  RADIO_CCACTRL_CCAMODE_WIDTH = 3,
  RADIO_CCACTRL_CCAMODE_POS = 0,
  RADIO_CCACTRL_CCAMODE_EDMODE = 0,
  RADIO_CCACTRL_CCAMODE_CARRIERMODE = 1,
  RADIO_CCACTRL_CCAMODE_CARRIERANDEDMODE = 2,
  RADIO_CCACTRL_CCAMODE_CARRIEROREDMODE = 3,
  RADIO_CCACTRL_CCAMODE_EDMODETESTMODE = 4,
  RADIO_CCACTRL_CCAEDTHRES_WIDTH = 8,
  RADIO_CCACTRL_CCAEDTHRES_POS = 8,
  RADIO_CCACTRL_CCACORRTHRES_WIDTH = 8,
  RADIO_CCACTRL_CCACORRTHRES_POS = 16,
  RADIO_CCACTRL_CCACORRCNT_WIDTH = 8,
  RADIO_CCACTRL_CCACORRCNT_POS = 24,
};


typedef enum {
  RADIO_DISABLED = 0,
  RADIO_RXRU = 1,
  RADIO_RXIDLE = 2,
  RADIO_RX = 3,
  RADIO_RXDISABLE = 4,
  RADIO_TRRU = 9,
  RADIO_TXIDLE = 10,
  RADIO_TX = 11,
  RADIO_TXDISABLE = 12 
} RadioState;


typedef struct __attribute__((packed)){
  uint32_t txEn;
  uint32_t rxEn;
  uint32_t start;
  uint32_t stop;
  uint32_t disable;
  uint32_t rssiStart;
  uint32_t rssiStop;
  uint32_t bcStart;
  uint32_t bcStop;
  uint32_t edStart;
  uint32_t edStop;
  uint32_t ccaStart;
  uint32_t ccaStop;
} RadioTask;

typedef struct __attribute__((packed)){
  uint32_t ready;
  uint32_t address;
  uint32_t payload;
  uint32_t end;
  uint32_t disabled;
  uint32_t devMatch;
  uint32_t devMiss;
  uint32_t rssiEnd;
  uint64_t _reserved0;
  uint32_t bcMatch;
  uint32_t _reserved1;
  uint32_t crcOk;
  uint32_t crcError;
  uint32_t frameStart;
  uint32_t edEnd;
  uint32_t edStopped;
  uint32_t ccaIdle;
  uint32_t ccaBusy;
  uint32_t ccaStopped;
  uint32_t rateBoost;
  uint32_t txReady;
  uint32_t rxReady;
  uint32_t mhrMatch;
  uint64_t _reserved2;
  uint32_t _reserved3;
  uint32_t phyEnd;
  uint32_t ctePresent;
} RadioEvent;

typedef struct __attribute__((packed)){
  uint32_t intenSet;
  uint32_t intenClr;
} RadioInterrupt;

typedef struct __attribute__((packed)){
  uint32_t base_segment[8];
  uint32_t prefix[8];
} RadioDeviceAddress;


RadioTask volatile* const pRadioTask = (RadioTask*) RADIO_BASE;
RadioEvent volatile* const pRadioEvent = (RadioEvent*) RADIO_BASE + 0x100;
RadioInterrupt volatile* const pRadioInterrupt = (RadioInterrupt*) RADIO_BASE + 0x304;
RadioDeviceAddress volatile* const pRadioDeviceAddress = (RadioDeviceAddress*) RADIO_BASE + 0x600;


void radioResetEvents(void);
void radioDisable(void);
void radioShortsReset(void);
void radioDeviceAddressReset(void);


void radioInit(void){
  radioResetEvents();
  radioDisable(); 
  radioShortsReset();
  RADIO_POWER = 0;
  sleep(100);
  RADIO_POWER = 1;
  sleep(100);
  return;
}

void radioReset(void){
  radioInit();
  return;
}


RadioState radioGetState(void){
  RadioState state = (RadioState) RADIO_STATE;
  return state;
}

void radioDisable(void){
  if (radioGetState() == RADIO_DISABLED){
    return;
  }
  pRadioEvent->disabled = 0;
  pRadioTask->disable = 1;
  while (!(pRadioEvent->disabled)){}
  return;
}

void radioResetEvents(void){
  pRadioEvent->ready = 0;
  pRadioEvent->address = 0;
  pRadioEvent->payload = 0;
  pRadioEvent->end = 0;
  pRadioEvent->disabled = 0;
  pRadioEvent->devMatch = 0;
  pRadioEvent->devMiss = 0;
  pRadioEvent->rssiEnd = 0;
  pRadioEvent->bcMatch = 0;
  pRadioEvent->crcOk = 0;
  pRadioEvent->crcError = 0;
  pRadioEvent->frameStart = 0;
  pRadioEvent->edEnd = 0;
  pRadioEvent->edStopped = 0;
  pRadioEvent->ccaIdle = 0;
  pRadioEvent->ccaBusy = 0;
  pRadioEvent->ccaStopped = 0;
  pRadioEvent->rateBoost = 0;
  pRadioEvent->txReady = 0;
  pRadioEvent->mhrMatch = 0;
  pRadioEvent->phyEnd = 0;
  pRadioEvent->ctePresent = 0;
  return;
}

void radioShortsReset(void){
  CLR_BIT(RADIO_SHORTS, RADIO_SHORTS_READY_START_POS);
  CLR_BIT(RADIO_SHORTS, RADIO_SHORTS_END_DISABLE_POS);
  CLR_BIT(RADIO_SHORTS, RADIO_SHORTS_DISABLE_TXEN_POS);
  CLR_BIT(RADIO_SHORTS, RADIO_SHORTS_DISABLE_RXEN_POS);
  CLR_BIT(RADIO_SHORTS, RADIO_SHORTS_ADDRESS_RSSISTART_POS);
  CLR_BIT(RADIO_SHORTS, RADIO_SHORTS_END_START_POS);
  CLR_BIT(RADIO_SHORTS, RADIO_SHORTS_ADDRESS_BCSTART_POS);
  CLR_BIT(RADIO_SHORTS, RADIO_SHORTS_DISABLED_RSSISTOP_POS);
  CLR_BIT(RADIO_SHORTS, RADIO_SHORTS_RXREADY_CCASTART_POS);
  CLR_BIT(RADIO_SHORTS, RADIO_SHORTS_CCAIDLE_TXEN_POS);
  CLR_BIT(RADIO_SHORTS, RADIO_SHORTS_CCABUSY_DISABLE_POS);
  CLR_BIT(RADIO_SHORTS, RADIO_SHORTS_FRAMESTART_BCSTART_POS);
  CLR_BIT(RADIO_SHORTS, RADIO_SHORTS_READY_EDSTART_POS);
  CLR_BIT(RADIO_SHORTS, RADIO_SHORTS_EDEND_DISABLE_POS);
  CLR_BIT(RADIO_SHORTS, RADIO_SHORTS_CCAIDLE_STOP_POS);
  CLR_BIT(RADIO_SHORTS, RADIO_SHORTS_TXREADY_START_POS);
  CLR_BIT(RADIO_SHORTS, RADIO_SHORTS_RXREADY_START_POS);
  CLR_BIT(RADIO_SHORTS, RADIO_SHORTS_PHYEND_DISABLE_POS);
  CLR_BIT(RADIO_SHORTS, RADIO_SHORTS_PHYEND_START_POS);
  return;
}

void radioDeviceAddressReset(void){
  for (uint8_t i = 0; i < 8; i++){
    pRadioDeviceAddress->base_segment[i] = 0;
    pRadioDeviceAddress->prefix[i] = 0;
  }
  return;
}

