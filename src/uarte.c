
#include <stdint.h>
#include "uarte.h"
#include "macros.h"
#include "utils.h"


#define UARTE0_BASE 0x40002000

#define UARTE0_SHORTS REG(uint32_t, UARTE0_BASE, 0x200)
#define UARTE0_ERRORSRC REG(uint32_t, UARTE0_BASE, 0x480)
#define UARTE0_ENABLE REG(uint32_t, UARTE0_BASE, 0x500)
#define UARTE0_BAUDRATE REG(uint32_t, UARTE0_BASE, 0x524)
#define UARTE0_CONFIG REG(uint32_t, UARTE0_BASE, 0x56C)

#define MAX_INPUT_LEN 32


enum {
  UARTE0_SHORTS_ENDRX_STARTRX_POS = 5,
  UARTE0_SHORTS_ENDRX_STOPRX_POS = 6
};

enum {
  UARTE0_ERRORSRC_OVERRUN_POS = 0,
  UARTE0_ERRORSRC_PARITY_POS = 1,
  UARTE0_ERRORSRC_FRAMING_POS = 2,
  UARTE0_ERRORSRC_BREAK_POS = 3
};

enum {
  UARTE0_ENABLE_DISABLED = 0,
  UARTE0_ENABLE_ENABLED = 8,
  UARTE0_ENABLE_WIDTH = 4,
  UARTE0_ENABLE_POS = 0
};

enum {
  UARTE0_BAUDRATE_9600 = 0x00275000,
  UARTE0_BAUDRATE_115200 = 0x01D60000,
};

enum {
  UARTE0_CONFIG_PARITY_EXCLUDED = 0,
  UARTE0_CONFIG_PARITY_INCLUDED = 7,
  UARTE0_CONFIG_PARITY_WIDTH = 3,
  UARTE0_CONFIG_PARITY_POS = 1,
  UARTE0_CONFIG_HWFC_POS = 0,
  UARTE0_CONFIG_STOP_POS = 4,
  UARTE0_CONFIG_PARITYTYPE_POS = 8
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


UarteTask* pUarteTask = (UarteTask*) UARTE0_BASE;
UarteEvent* pUarteEvent = (UarteEvent*) (UARTE0_BASE + 0x100);
UarteInterrupt* pUarteInterrupt = (UarteInterrupt*) (UARTE0_BASE + 0x300);
UartePin* pUartePin = (UartePin*) (UARTE0_BASE + 0x508);
UarteData* pUarteData = (UarteData*) (UARTE0_BASE + 0x534);




enum Baudrate {
  BAUD_9600 = 0x00275000,
  BAUD_115200 = 0x01D60000
};


enum HWFC {
  HWFC_ENABLE = 1,
  HWFC_DISABLE = 0
};


Uarte Uarte_create(Gpio* txPin, Gpio* rxPin){
  Uarte uarte = {
    MAX_INPUT_LEN,
    {txPin->port, txPin->pin},
    {rxPin->port, rxPin->pin}
  };
  Uarte_init(&uarte);
  return uarte;
}


void Uarte_enable(Uarte* uarte){
  UARTE0_ENABLE = 8;
}


void Uarte_disable(Uarte* uarte){
  UARTE0_ENABLE = 0;
}


void Uarte_setTxPin(Uarte* self){
  uint8_t port = self->txPin.port;
  uint8_t pin = self->txPin.pin;
  UARTE0_PSELTXD = ((port << 5) + pin);
  return;
}


void Uarte_setRxPin(Uarte* self){
  uint8_t port = self->rxPin.port;
  uint8_t pin = self->rxPin.pin;
  UARTE0_PSELRXD = ((port << 5) + pin);
  return;
}


void Uarte_init(Uarte* self){
  Uarte_disable(self);
  Uarte_setBaudrate9600(self);
  Uarte_setParityNone(self);
  Uarte_setTxPin(self);
  Uarte_setRxPin(self);
  Uarte_enable(self);
  return;
}


static void Uarte_txReset(void){
  UARTE0_TXDRDY = 0;
  UARTE0_STARTTX = 0;
  UARTE0_ENDTX = 0;
  return;
}


static void Uarte_setBuffSize(uint8_t buffSize){
  UARTE0_TXDMAXCNT = buffSize;
  return;
}


static void Uarte_setTxPtr(const char* str){
  UARTE0_TXD = (uint32_t) str;
  return;
}


static void Uarte_triggerTx(){
  UARTE0_STARTTX = 1;
  return;
}


static void Uarte_block(){
  while (!UARTE0_ENDTX){}
  return;
}


void Uarte_writeChar(Uarte* self, const char* ch){
  Uarte_txReset();
  Uarte_setBuffSize(1);
  Uarte_setTxPtr(ch);
  Uarte_triggerTx();
  Uarte_block();
  return;
}


void Uarte_writeStr(Uarte* self, const char* str){
  Uarte_txReset();
  uint32_t buffSize = strLength(str);
  Uarte_setBuffSize(buffSize);
  char strBuffer[buffSize];
  for (int i=0; i<buffSize; i++){
    strBuffer[i] = *(str + i);
  }
  Uarte_setTxPtr(strBuffer);
  Uarte_triggerTx();
  Uarte_block();
  return;
}


static uint8_t numberOfDigits(uint32_t integer, uint8_t base){
  uint8_t n_digits = 0;
  do {
      n_digits++; 
      integer /= base;
  } while (integer > 0);
  return n_digits;
}


void Uarte_writeInt(Uarte* self, uint32_t integer, uint8_t base){
  static const char dec[] = "0123456789ABCDEF";
  uint8_t nDigits = numberOfDigits(integer, base);
  char digits[nDigits+1];
  digits[nDigits] = '\0';
  while (nDigits-- > 0){
      digits[nDigits] = dec[integer%base];
      integer /= base;
  }
  Uarte_writeStr(self, digits);
  return;
}


void Uarte_input(Uarte* self, char* input){
  char rxBuff[1];
  int i = 0;
  while (i < MAX_INPUT_LEN){
    Uarte_getChar(self, rxBuff);
    if (*rxBuff == '\r'){
      input[i] = '\0';
      /* serial_flush(); */
      return;
    }
    Uarte_writeChar(self, rxBuff);
    input[i] = rxBuff[0];
    i++;
  }
  return;
}


static void Uarte_rxReset(){
  UARTE0_RXDRDY = 0;
  UARTE0_RXTO = 0;
  UARTE0_STARTRX = 0;
  UARTE0_ENDRX = 0;
  return;
}


static void Uarte_rxRead(char* ch){
  UARTE0_RXDMAXCNT = 1;
  UARTE0_RXD = (uint32_t) ch;
  UARTE0_STARTRX = 1;
  return;
}


static void Uarte_rxBlock(){
  while (!UARTE0_ENDRX || !UARTE0_RXDRDY){}
  return;
}


static void Uarte_rxStop(){
  UARTE0_STOPRX = 1;
  return;
}


void Uarte_getChar(Uarte* self, char* ch){
  Uarte_rxReset();
  Uarte_rxRead(ch);
  Uarte_rxBlock();
  Uarte_rxStop();
  return;
}


void Uarte_listenChar(Uarte* self, char* ch){
  Uarte_rxReset();
  Uarte_rxRead(ch);
  return;
}


void Uarte_endl(Uarte* self){
  Uarte_writeStr(self, "\n\r");
  return;
}


void Uarte_flush(Uarte* self){
  UARTE0_FLUSHRX = 1;
  return;
}


void Uarte_setBaudrate9600(Uarte* self){
  UARTE0_BAUDRATE = BAUD_9600;
  return;
}


void Uarte_setBaudrate115200(Uarte* self){
  UARTE0_BAUDRATE = BAUD_115200;
  return;
}


void Uarte_setParityNone(Uarte* self){
  SET_FIELD(UARTE0_CONFIG, 1, 3, 0);
  return;
}


void Uarte_setParityEven(Uarte* self){
  SET_FIELD(UARTE0_CONFIG, 1, 3, 7);
  SET_FIELD(UARTE0_CONFIG, 8, 1, 0);
  return;
}


void Uarte_setParityOdd(Uarte* self){
  SET_FIELD(UARTE0_CONFIG, 1, 3, 7);
  SET_FIELD(UARTE0_CONFIG, 8, 1, 1);
  return;
}


void Uarte_setHwfcEnable(Uarte* self){
  SET_FIELD(UARTE0_CONFIG, 0, 1, HWFC_ENABLE);
  return;
}


void Uarte_setHwfcDisable(Uarte* self){
  SET_FIELD(UARTE0_CONFIG, 0, 1, HWFC_DISABLE);
  return;
}
