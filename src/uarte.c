
#include "uarte.h"
#include "utils.h"


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


void Uarte_writeChar(const char* ch){
  Uarte_txReset();
  Uarte_setBuffSize(1);
  Uarte_setTxPtr(ch);
  Uarte_triggerTx();
  Uarte_block();
  return;
}


void Uarte_writeStr(const char* str){
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


void Uarte_writeInt(uint32_t integer, uint8_t base){
  static const char dec[] = "0123456789ABCDEF";
  uint8_t nDigits = numberOfDigits(integer, base);
  char digits[nDigits+1];
  digits[nDigits] = '\0';
  while (nDigits-- > 0){
      digits[nDigits] = dec[integer%base];
      integer /= base;
  }
  Uarte_writeStr(digits);
  return;
}


void Uarte_input(char* input){
  char rxBuff[1];
  int i = 0;
  while (i < MAX_INPUT_LEN){
    Uarte_getChar(rxBuff);
    if (*rxBuff == '\r'){
      input[i] = '\0';
      /* serial_flush(); */
      return;
    }
    Uarte_writeChar(rxBuff);
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


void Uarte_getChar(char* ch){
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
  Uarte_writeStr("\n\r");
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
