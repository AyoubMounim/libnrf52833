
#include "nrf52833/radio.h"
#include "macros.h"
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

