
#include "nrf52833/radio.h"
#include "macros.h"
#include <stdint.h>


#define RADIO_BASE 0x40001000

#define SHORTS REG(uint32_t, RADIO_BASE, 0x200)
#define RXMATCH REG(uint32_t, RADIO_BASE, 0x408)
#define RXCRC REG(uint32_t, RADIO_BASE, 0x40C)
#define DAI REG(uint32_t, RADIO_BASE, 0x410)
#define PDUSTAT REG(uint32_t, RADIO_BASE, 0x414)
#define CTESTATUS REG(uint32_t, RADIO_BASE, 0x44C)
#define DFESTATUS REG(uint32_t, RADIO_BASE, 0x458)
#define PACKETPTR REG(uint32_t, RADIO_BASE, 0x504)
#define FREQUENCY REG(uint32_t, RADIO_BASE, 0x508)
#define TXPOWER REG(uint32_t, RADIO_BASE, 0x50C)
#define MODE REG(uint32_t, RADIO_BASE, 0x510)
#define PCNF0 REG(uint32_t, RADIO_BASE, 0x514)
#define PCNF1 REG(uint32_t, RADIO_BASE, 0x518)
#define BASE0 REG(uint32_t, RADIO_BASE, 0x51C)
#define BASE1 REG(uint32_t, RADIO_BASE, 0x520)
#define PREFIX0 REG(uint32_t, RADIO_BASE, 0x524)
#define PREFIX1 REG(uint32_t, RADIO_BASE, 0x528)
#define TXADDRESS REG(uint32_t, RADIO_BASE, 0x52C)
#define RXADDRESSES REG(uint32_t, RADIO_BASE, 0x530)
#define CRCCNF REG(uint32_t, RADIO_BASE, 0x534)
#define CRCPOLY REG(uint32_t, RADIO_BASE, 0x538)
#define CRCINIT REG(uint32_t, RADIO_BASE, 0x53C)
#define TIFS REG(uint32_t, RADIO_BASE, 0x544)
#define RSSISAMPLE REG(uint32_t, RADIO_BASE, 0x548)
#define STATE REG(uint32_t, RADIO_BASE, 0x550)
#define DATAWHITEIV REG(uint32_t, RADIO_BASE, 0x554)
#define BCC REG(uint32_t, RADIO_BASE, 0x560)
#define DACNF REG(uint32_t, RADIO_BASE, 0x640)
#define MHRMATCHCONGIG REG(uint32_t, RADIO_BASE, 0x644)
#define MHRMATCHMAS REG(uint32_t, RADIO_BASE, 0x648)
#define MODECNF0 REG(uint32_t, RADIO_BASE, 0x650)
#define SFD REG(uint32_t, RADIO_BASE, 0x660)
#define EDCNT REG(uint32_t, RADIO_BASE, 0x664)
#define EDSAMPLE REG(uint32_t, RADIO_BASE, 0x668)
#define CCACTRL REG(uint32_t, RADIO_BASE, 0x66C)
#define DFEMODE REG(uint32_t, RADIO_BASE, 0x900)
#define CTEINLINECONF REG(uint32_t, RADIO_BASE, 0x904) 
#define DFECTRL1 REG(uint32_t, RADIO_BASE, 0x910) 
#define DFECTRL2 REG(uint32_t, RADIO_BASE, 0x914) 
#define SWITCHPATTERN REG(uint32_t, RADIO_BASE, 0x924) 
#define CLEARPATTERN REG(uint32_t, RADIO_BASE, 0x924) 
#define DFEPACKETPTR REG(uint32_t, RADIO_BASE, 0x950)
#define DFEPACKETMAXCNT REG(uint32_t, RADIO_BASE, 0x954)
#define DFEPACKETAMOUNT REG(uint32_t, RADIO_BASE, 0x958)
#define POWER REG(uint32_t, RADIO_BASE, 0xFFC)

#define PSELDFEGPIO(n) REG(uint32_t, RADIO_BASE, 0x930 + (n))


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

